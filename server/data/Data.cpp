/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Data
*/

#include <cstring>
#include "Data.hpp"

namespace data {

Data::Data()
{}

Data::~Data()
{}

protocol::serverMessage Data::interpretMessage(protocol::connectionMessage &message) noexcept
{
	std::string clientName = std::string(message.clientName);
	protocol::serverMessage response;

	response.headerId = protocol::SERVER_RESPONSE;
	response.response = 0;
	strcpy(response.ip, "");
	response.port = 0;

	if (message.headerId == protocol::CONNECT)
		response.response = addClient(message);
	else if (message.headerId == protocol::DISCONNECT)
		response.response = removeClient(clientName);

	return response;
}

protocol::serverMessage Data::interpretMessage(protocol::callMessage &message) noexcept
{
	std::string clientName = std::string(message.clientName);
	std::string contactName = std::string(message.contactName);
	protocol::serverMessage response;

	response.headerId = protocol::SERVER_RESPONSE;
	response.response = 0;
	strncpy(response.ip, "", protocol::IP_LENGTH);
	response.port = 0;

	if (message.headerId == protocol::REQUEST_CALL) {
		if (getClientState(clientName) != protocol::AVAILABLE)
			return response;

		std::string ip = getClientIp(contactName);
		unsigned short port = getClientPort(contactName);
		if (ip == "ERROR" || port == 0)
			return response;

		for (int i = 0; i < protocol::IP_LENGTH; i++)
			response.ip[i] = ip.c_str()[i];
		response.ip[protocol::IP_LENGTH] = '\0';

		response.port = port;

		response.response = setClientState(clientName, protocol::UNAVAILABLE);
		response.response = setClientState(contactName, protocol::UNAVAILABLE);
	} else if (message.headerId == protocol::NO_REPLY || message.headerId == protocol::END_CALL) {
		response.response = setClientState(clientName, protocol::AVAILABLE);
		response.response = setClientState(contactName, protocol::AVAILABLE);
	}

	return response;
}

protocol::infoResponseMessage Data::interpretMessage(protocol::infoMessage&) noexcept
{
	protocol::infoResponseMessage response;
	response.headerId = protocol::SERVER_RESPONSE;

	std::string clients = getAllClientNames();

	response.nextMessageLength = clients.size();
	response.contactNames = strdup(clients.c_str());

	return response;
}

bool Data::addClient(protocol::connectionMessage &message) noexcept
{
	if (_clients.find(std::string(message.clientName)) == _clients.end()) {

		client newClient;

		newClient.clientName = std::string(message.clientName);
		newClient.state = protocol::AVAILABLE;
		newClient.ip = std::string(message.ip);
		newClient.port = message.port;

		_clients.emplace(newClient.clientName, newClient);
		return true;
	}

	return false;
}

bool Data::removeClient(std::string &clientName) noexcept
{
	if (_clients.find(clientName) != _clients.end()) {
		_clients.erase(clientName);
		return true;
	}

	return false;
}

std::string Data::getAllClientNames() const noexcept
{
	std::string clients("");
	int index = 0;

	for (auto &client : _clients) {
		if (index != 0)
			clients += "|";

		clients += client.first;
		index++;
	}

	return clients;
}

std::string Data::getClientIp(std::string &clientName) const noexcept
{
	if (_clients.find(clientName) != _clients.end())
		return _clients.at(clientName).ip;

	return std::string("ERROR");
}

unsigned short Data::getClientPort(std::string &clientName) const noexcept
{
	if (_clients.find(clientName) != _clients.end())
		return _clients.at(clientName).port;

	return (unsigned short)0;
}

protocol::state Data::getClientState(std::string &clientName) const noexcept
{
	if (_clients.find(clientName) != _clients.end())
		return _clients.at(clientName).state;

	return protocol::UNAVAILABLE;
}

bool Data::setClientState(std::string &clientName, protocol::state state) noexcept
{
	if (_clients.find(clientName) != _clients.end()) {
		_clients.at(clientName).state = state;
		return true;
	}

	return false;
}

}
