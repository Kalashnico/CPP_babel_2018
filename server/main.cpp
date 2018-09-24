/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Babel server main
*/

#include <iostream>
#include "tcpserver/BoostTcpServer.hpp"

int main()
{
	try {
		boost::asio::io_context ioContext;
		unsigned short port = 6969;

		tcpserver::BoostTcpServer server(ioContext, port);

		ioContext.run();
	} catch(std::exception e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}