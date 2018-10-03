/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TCP Server Boost implementation
*/

#pragma once

#include <boost/asio.hpp>
#include "ATcpServer.hpp"
#include "Data.hpp"

namespace tcpserver {

class BoostTcpServer : public ATcpServer {
	public:
		BoostTcpServer(boost::asio::io_context&, unsigned short);
		~BoostTcpServer();

		void run() noexcept final;

	private:
		boost::asio::ip::tcp::acceptor _acceptor;
		data::Data *_data;

		void accept() noexcept;
};

}
