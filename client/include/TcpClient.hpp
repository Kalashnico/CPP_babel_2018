/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TcpClient
*/

#pragma once

#include <QtNetwork/QtNetwork>
#include "Protocol.hpp"

namespace tcpclient {

class TcpClient {
	public:
		TcpClient();
		~TcpClient();

		bool send(protocol::connectionMessage&) noexcept;
		bool send(protocol::callMessage&) noexcept;

		protocol::serverMessage receive() noexcept;

	private:
		protocol::Protocol _protocol;
		QTcpSocket _socket;

		bool send(protocol::PACKET&) noexcept;
};

}