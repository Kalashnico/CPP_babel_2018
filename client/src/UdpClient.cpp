/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TcpClient
*/

#include "UdpClient.hpp"
#include <iostream>

namespace udpclient {

UdpClient::UdpClient(std::string &username, unsigned short port)
	: _username(username), _myPort(port)
{
	_socket.bind(QHostAddress::AnyIPv4, port);
}

UdpClient::~UdpClient()
{
	_socket.close();
}

void UdpClient::setContactInfo(std::string &contact, std::string &ip, unsigned short port) noexcept
{
	_currentContact = contact;
	_currentContactIp = ip;
	_currentContactPort = port;
}

std::string UdpClient::getContactName() const noexcept
{
	return _currentContact;
}

void UdpClient::sendCallRequestDatagram() noexcept
{
	QByteArray datagram;
	QDataStream out(&datagram, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Version::Qt_5_11);

	protocol::callMessage callRequestMessage;
	callRequestMessage.headerId = protocol::REQUEST_CALL;
	strcpy(callRequestMessage.clientName, _username.c_str());
	strcpy(callRequestMessage.contactName, _currentContact.c_str());

	auto packet = _protocol.encode(callRequestMessage);
	out.writeRawData((char*)packet, 1024);

	_socket.writeDatagram(datagram, datagram.size(), QHostAddress(QString::fromStdString(_currentContactIp)), _currentContactPort);
}

void UdpClient::sendResponseDatagram(bool response) noexcept
{
	QByteArray datagram;
	QDataStream out(&datagram, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Version::Qt_5_11);

	protocol::serverMessage responseMessage;
	responseMessage.headerId = protocol::SERVER_RESPONSE;
	responseMessage.response = response;
	strcpy(responseMessage.ip, "");
	responseMessage.port = 0;

	auto packet = _protocol.encode(responseMessage);
	out.writeRawData((char*)packet, 1024);

	_socket.writeDatagram(datagram, datagram.size(), QHostAddress(QString::fromStdString(_currentContactIp)), _currentContactPort);
}

void UdpClient::sendAudioDatagram(char *data, unsigned short frameBuffer) noexcept
{
	QByteArray datagram;
	QDataStream out(&datagram, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Version::Qt_5_11);

	protocol::audioMessage encodedAudioMessage;
	encodedAudioMessage.headerId = protocol::AUDIO;
	encodedAudioMessage.data = strdup(data);
	encodedAudioMessage.length = frameBuffer;

	auto packet = _protocol.encode(encodedAudioMessage);
	out.writeRawData((char*)packet, 1024);

	_socket.writeDatagram(datagram, datagram.size(), QHostAddress(QString::fromStdString(_currentContactIp)), _currentContactPort);

}

callRequest UdpClient::readPendingRequestDatagrams() noexcept
{

	callRequest request;
	request.caller = "";
	request.ip = "";
	request.port = 0;

	if (_socket.hasPendingDatagrams()) {
		auto datagram = _socket.receiveDatagram();
		protocol::PACKET_BUFFER packet;
		for (int i = 0; i < protocol::PACKET_SIZE; i++)
			packet[i] = datagram.data()[i];

		auto decodedMessage = _protocol.decodeCallMessage(packet);

		if (decodedMessage.headerId != protocol::REQUEST_CALL)
			return request;

		std::string caller(decodedMessage.clientName);
		std::string ip(datagram.senderAddress().toString().toStdString());

		request.caller = caller;
		request.ip = ip;
		request.port = datagram.senderPort();
		return request;
	}

	return request;
}

char UdpClient::readPendingResponseDatagrams() noexcept
{
	if (_socket.hasPendingDatagrams()) {
		auto datagram = _socket.receiveDatagram();
		protocol::PACKET_BUFFER packet;
		for (int i = 0; i < protocol::PACKET_SIZE; i++)
			packet[i] = datagram.data()[i];

		auto decodedMessage = _protocol.decodeServerMessage(packet);

		if (decodedMessage.headerId != protocol::SERVER_RESPONSE)
			return -1;

		return decodedMessage.response;
	}

	return -1;
}

protocol::audioMessage UdpClient::readPendingAudioDatagrams() noexcept
{
	protocol::audioMessage audioMessage;
	audioMessage.headerId = protocol::AUDIO;
	audioMessage.data = strdup("");
	audioMessage.length = 0;

	if (_socket.hasPendingDatagrams()) {
		auto datagram = _socket.receiveDatagram();
		protocol::PACKET_BUFFER packet;
		for (int i = 0; i < protocol::PACKET_SIZE; i++)
			packet[i] = datagram.data()[i];

		//TODO: 960 is frame buffer, this needs to be set as const somewhere
		auto decodedMessage = _protocol.decodeAudioMessage(packet, 960);

		if (decodedMessage.headerId != protocol::AUDIO)
			return audioMessage;

		audioMessage.data = strdup(decodedMessage.data);
		audioMessage.length = decodedMessage.length;
	}

	return audioMessage;
}

}
