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
				std::cout << '\n' << "Received new message:" << std::endl;

				auto messageType = _protocol.getMessageType(_packet);
				protocol::serverMessage message;

				if (messageType == protocol::CONNECTION) {
					auto decodedMessage = _protocol.decodeConnectionMessage(_packet);

					strcpy(decodedMessage.ip, _clientIp.c_str());

					std::cout << "\tMessage type: CONNECTION" << std::endl;
					std::cout << "\tHeader: " << headerNames[decodedMessage.headerId] << std::endl;
					std::cout << "\tUsername: " << decodedMessage.clientName << std::endl;
					std::cout << "\tIP: " << decodedMessage.ip << std::endl;
					std::cout << "\tPort: " << decodedMessage.port << std::endl;

					message =_data.interpretMessage(decodedMessage);
					writePacket(message);
				} else if (messageType == protocol::CALL) {
					auto decodedMessage = _protocol.decodeCallMessage(_packet);

					std::cout << "\tMessage type: CALL" << std::endl;
					std::cout << "\tHeader: " << headerNames[decodedMessage.headerId] << std::endl;
					std::cout << "\tUsername: " << decodedMessage.clientName << std::endl;
					std::cout << "\tContact name: " << decodedMessage.contactName << std::endl;

					message =_data.interpretMessage(decodedMessage);
					writePacket(message);
				}
			}
	});
}

void BoostTcpSession::writePacket(protocol::serverMessage &message) noexcept
{
	std::cout << "Reply:" << std::endl;
	std::cout << "\tMessage type: SERVER" << std::endl;
	std::string response = "";
	if (message.response == -1)
		response += "-1";
	else if (message.response == 0)
		response += "0";
	else if (message.response == 1)
		response += "1";
	std::cout << "\tHeader: " << headerNames[message.headerId] << std::endl;
	std::cout << "\tResponse: " << response << std::endl;
	std::cout << "\tIP: " << message.ip << std::endl;
	std::cout << "\tPort: " << message.port << std::endl;

	auto encodedMessage = _protocol.encode(message);

	auto self(shared_from_this());
	boost::asio::async_write(_socket, boost::asio::buffer(encodedMessage, protocol::PACKET_SIZE),
		[this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec)
				readPacket();
	});
}

}
