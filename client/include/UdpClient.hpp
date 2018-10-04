/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TcpClient
*/

#pragma once

#include <QtNetwork/QtNetwork>
#include "Protocol.hpp"

namespace udpclient {

class UdpClient {
	public:
		UdpClient();
		~UdpClient();

		/*bool send(protocol::connectionMessage&) noexcept;
		bool send(protocol::callMessage&) noexcept;
		bool send(protocol::infoMessage&) noexcept;

		protocol::serverMessage receive() noexcept;
		protocol::infoResponseMessage receiveClients() noexcept;*/

	private:
		protocol::Protocol _protocol;
		QUdpSocket _socket;

		//bool send(protocol::PACKET&) noexcept;
};

}
