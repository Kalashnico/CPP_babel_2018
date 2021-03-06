/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TCP Session for Boost
*/

#pragma once

#include <boost/asio.hpp>
#include "Data.hpp"

namespace tcpserver {

class BoostTcpSession : public std::enable_shared_from_this<BoostTcpSession> {
	public:
		BoostTcpSession(boost::asio::ip::tcp::socket, data::Data*);
		~BoostTcpSession();

		void start() noexcept;

	protected:
	private:
		boost::asio::ip::tcp::socket _socket;
		protocol::Protocol _protocol;
		data::Data *_data;

		std::string _clientIp;

		protocol::PACKET_BUFFER _packet;

		const char *headerNames[8] = { "CONNECT", "DISCONNECT", "GET_CONTACTS", "REQUEST_CALL", "NO_REPLY", "END_CALL", "AUDIO", "SERVER_RESPONSE" };

		void readPacket() noexcept;
		void writePacket(protocol::serverMessage&) noexcept;
		void writeInfoResponsePacket(protocol::infoResponseMessage, bool) noexcept;
};

}
