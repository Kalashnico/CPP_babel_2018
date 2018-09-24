/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Abstract TCP Server
*/

#pragma once

namespace tcpserver {

class ATcpServer {
	public:
		ATcpServer(unsigned short);
		~ATcpServer();

		virtual void run() noexcept = 0;

	protected:
		unsigned short _port;
};

}
