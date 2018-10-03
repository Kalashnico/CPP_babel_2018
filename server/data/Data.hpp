/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Data
*/

#pragma once

#include <unordered_map>
#include "Protocol.hpp"

namespace data {

struct client {
	std::string clientName;
	protocol::state state;
	std::string ip;
	unsigned short port;
};

class Data {
	public:
		Data();
		~Data();

		protocol::serverMessage interpretMessage(protocol::connectionMessage&) noexcept;
		protocol::serverMessage interpretMessage(protocol::callMessage&) noexcept;
		protocol::infoResponseMessage interpretMessage(protocol::infoMessage&) noexcept;

	private:
		std::unordered_map<std::string, client> _clients;

		bool addClient(protocol::connectionMessage&) noexcept;
		bool removeClient(std::string&) noexcept;

		std::string getAllClientNames() const noexcept;

		std::string getClientIp(std::string&) const noexcept;
		unsigned short getClientPort(std::string&) const noexcept;
		protocol::state getClientState(std::string&) const noexcept;
		bool setClientState(std::string&, protocol::state) noexcept;

};

}
