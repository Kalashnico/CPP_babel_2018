/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TcpClient
*/

#include "TcpClient.hpp"

namespace tcpclient {

TcpClient::TcpClient()
{
	_socket.connectToHost("87.231.49.182", 6969);
}

TcpClient::~TcpClient()
{
	_socket.disconnectFromHost();
}

bool TcpClient::send(protocol::connectionMessage &message) noexcept
{
	auto packet = _protocol.encode(message);
	return (send(packet));
}

bool TcpClient::send(protocol::callMessage &message) noexcept
{
	auto packet = _protocol.encode(message);
	return(send(packet));
}

bool TcpClient::send(protocol::infoMessage &message) noexcept
{
	auto packet = _protocol.encode(message);
	return(send(packet));
}

bool TcpClient::send(protocol::PACKET &packet) noexcept
{
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Version::Qt_5_11);

	out.writeRawData((char*)packet, 1024);

	if (_socket.waitForConnected()) {
		_socket.write(data);
		_socket.flush();
		return true;
	}

	return false;
}

protocol::serverMessage TcpClient::receive() noexcept
{
	if (_socket.waitForReadyRead()) {
		auto received = _socket.readAll();
		protocol::PACKET_BUFFER packet;
		for (int i = 0; i < protocol::PACKET_SIZE; i++)
			packet[i] = received.data()[i];

		return _protocol.decodeServerMessage(packet);
	}

	protocol::serverMessage error;
	error.headerId = protocol::SERVER_RESPONSE;
	error.response = -1;
	strcpy(error.ip, "");
	error.port = 0;
	return error;
}

protocol::infoResponseMessage TcpClient::receiveClients() noexcept
{
	if (_socket.waitForReadyRead()) {
		auto received = _socket.readAll();
		protocol::PACKET_BUFFER packet;
		for (int i = 0; i < protocol::PACKET_SIZE; i++)
			packet[i] = received.data()[i];

		auto response = _protocol.decodeInfoResponseMessage(packet, 0);

		if (_socket.waitForReadyRead()) {
			auto received = _socket.readAll();
			protocol::UINT8 packet[protocol::PACKET_SIZE + response.nextMessageLength];
			for (int i = 0; i < protocol::PACKET_SIZE + response.nextMessageLength; i++)
				packet[i] = received.data()[i];

			return _protocol.decodeInfoResponseMessage(packet, response.nextMessageLength);
		}
	}

	protocol::infoResponseMessage error;
	error.headerId = protocol::SERVER_RESPONSE;
	error.nextMessageLength = -1;
	error.contactNames = strdup("");
	return error;
}

}
