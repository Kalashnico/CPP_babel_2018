/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TcpClient
*/

#include "UdpClient.hpp"
#include <iostream>

namespace udpclient {

UdpClient::UdpClient(unsigned short port)
	: _myPort(port)
{
	_socket.bind(QHostAddress::AnyIPv4, port);
}

UdpClient::~UdpClient()
{
	_socket.close();
}

void UdpClient::setContactInfo(std::string &ip, unsigned short port) noexcept
{
	_currentContactIp = ip;
	_currentContactPort = port;
}

void UdpClient::sendDatagram() noexcept
{
	QByteArray datagram = "Cheese";

	std::cout << "Sending datagram to " << _currentContactIp << ":" << _currentContactPort << std::endl;

	_socket.writeDatagram(datagram, datagram.size(), QHostAddress(QString::fromStdString(_currentContactIp)), _currentContactPort);
}

void UdpClient::readPendingDatagrams() noexcept
{
	if (_socket.hasPendingDatagrams()) {
		auto datagram = _socket.receiveDatagram();
		std::string ip(datagram.senderAddress().toString().toStdString());
		std::string port(std::to_string(datagram.senderPort()));
		std::cout << "Received datagram from " << ip << ":" << port << std::endl;
		exit(0);
	}
}

}
