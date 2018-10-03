/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TCP Session for Boost
*/

#include "BoostTcpSession.hpp"
#include <iostream>

namespace tcpserver {

BoostTcpSession::BoostTcpSession(boost::asio::ip::tcp::socket socket)
	: _socket(std::move(socket))
{
	_clientIp = _socket.remote_endpoint().address().to_string();
}

BoostTcpSession::~BoostTcpSession()
{}

void BoostTcpSession::start() noexcept
{
	readPacket();
}

void BoostTcpSession::readPacket() noexcept
{
	auto self(shared_from_this());
	_socket.async_read_some(boost::asio::buffer(_packet, protocol::PACKET_SIZE),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				std::cout << std::endl << "Received new message: " << std::endl;

				auto messageType = _protocol.getMessageType(_packet);
				protocol::serverMessage message;

				if (messageType == protocol::CONNECTION) {
					auto decodedMessage = _protocol.decodeConnectionMessage(_packet);

					strcpy(decodedMessage.ip, _clientIp.c_str());

					std::cout << "Message type: CONNECTION" << std::endl;
					std::cout << "Header id: " << decodedMessage.headerId << std::endl;
					std::cout << "Username: " << decodedMessage.clientName << std::endl;
					std::cout << "IP: " << decodedMessage.ip << std::endl;
					std::cout << "Port: " << decodedMessage.port << std::endl;

					message =_data.interpretMessage(decodedMessage);
					writePacket(message);
				} else if (messageType == protocol::CALL) {
					auto decodedMessage = _protocol.decodeCallMessage(_packet);

					std::cout << "Message type: CALL" << std::endl;
					std::cout << "Header id: " << decodedMessage.headerId << std::endl;
					std::cout << "Username: " << decodedMessage.clientName << std::endl;
					std::cout << "Contact name: " << decodedMessage.contactName << std::endl;

					message =_data.interpretMessage(decodedMessage);
					writePacket(message);
				}
			}
	});
}

void BoostTcpSession::writePacket(protocol::serverMessage &message) noexcept
{
	auto encodedMessage = _protocol.encode(message);

	auto self(shared_from_this());
	boost::asio::async_write(_socket, boost::asio::buffer(encodedMessage, protocol::PACKET_SIZE),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec)
				readPacket();
	});
}

}
