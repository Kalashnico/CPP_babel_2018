/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Abstract TCP Server
*/

#pragma once

namespace tcpserver {

class ITcpServer {
	public:
		virtual ~ITcpServer() = default;

		virtual void run() noexcept = 0;
		virtual void accept() noexcept = 0;

	protected:
		unsigned short _port;
};

}
