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
			UdpClient(unsigned short);

			~UdpClient();

			void sendDatagram() noexcept;

			void readPendingDatagrams() noexcept;

			void setContactInfo(std::string &, unsigned short
			) noexcept;

		private:
			protocol::Protocol _protocol;
			QUdpSocket _socket;

			unsigned short _myPort;

			std::string _currentContactIp;
			unsigned short _currentContactPort;
	};
}
