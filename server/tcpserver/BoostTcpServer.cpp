/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** TCP Server Boost implementation
*/

#include <memory>
#include <boost/system/error_code.hpp>
#include "BoostTcpServer.hpp"
#include "BoostTcpSession.hpp"

namespace tcpserver {

BoostTcpServer::BoostTcpServer(boost::asio::io_context &ioContext, unsigned short port)
	: _acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
	_data(new data::Data())
{
	run();
}

BoostTcpServer::~BoostTcpServer()
{}

void BoostTcpServer::run() noexcept
{
	accept();
}

void BoostTcpServer::accept() noexcept
{
	_acceptor.async_accept(
		[this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
			if (!ec)
				std::make_shared<BoostTcpSession>(std::move(socket), _data)->start();

			accept();
		}
	);
}

}
