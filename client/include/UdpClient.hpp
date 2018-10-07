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


struct callRequest {
	std::string caller;
	std::string ip;
	unsigned short port;
};

class UdpClient {
	public:
		UdpClient(std::string &username, unsigned short);
		~UdpClient();

		void sendCallRequestDatagram() noexcept;
		void sendResponseDatagram(bool) noexcept;
		void sendAudioDatagram(unsigned char*, unsigned short) noexcept;

		callRequest readPendingRequestDatagrams() noexcept;
		char readPendingResponseDatagrams() noexcept;
		protocol::audioMessage readPendingAudioDatagrams(int) noexcept;

		void setContactInfo(std::string&, std::string&, unsigned short) noexcept;
		std::string getContactName() const noexcept;

	private:
		protocol::Protocol _protocol;
		QUdpSocket _socket;

		std::string _username;
		unsigned short _myPort;

		std::string _currentContact;
		std::string _currentContactIp;
		unsigned short _currentContactPort;
};

}
