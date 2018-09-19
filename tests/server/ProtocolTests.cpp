/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Protocol Tests
*/

#define CATCH_CONFIG_MAIN

#include <cstring>
#include "catch.hpp"
#include "Protocol.hpp"

TEST_CASE("Protocol connection message encode/decode", "[protocol]")
{
	auto protocol = new protocol::Protocol();
	protocol::connectionMessage message;

	SECTION("Setting up message") {
		message.headerId = protocol::DISCONNECT;
		strcpy(message.clientName, "Cheese");
		strcpy(message.ip, "10.10.10.10");
		message.port = 4242;

		REQUIRE(std::string(message.clientName) == "Cheese");
		REQUIRE(std::string(message.ip) == "10.10.10.10");

		SECTION("Encode and decode message") {
			auto encodedMessage = protocol->encode(message);
			protocol::PACKET_BUFFER recievedMessage;

			for (int i = 0; i < 1024; i++)
				recievedMessage[i] = encodedMessage[i];

			auto headerType = protocol->getMessageType(recievedMessage);

			REQUIRE(headerType == protocol::CONNECTION);

			auto decodedMessage = protocol->decodeConnectionMessage(recievedMessage);

			REQUIRE(decodedMessage.headerId == protocol::DISCONNECT);
			REQUIRE(std::string(decodedMessage.clientName) == "Cheese");
			REQUIRE(std::string(decodedMessage.ip) == "10.10.10.10");
			REQUIRE(decodedMessage.port == 4242);
		}
	}
}

TEST_CASE("Protocol call message encode/decode", "[protocol]")
{
	auto protocol = new protocol::Protocol();
	protocol::callMessage message;

	SECTION("Setting up message") {
		message.headerId = protocol::REQUEST_CALL;
		strcpy(message.clientName, "Foo");
		strcpy(message.contactName, "Bar");

		REQUIRE(std::string(message.clientName) == "Foo");
		REQUIRE(std::string(message.contactName) == "Bar");

		SECTION("Encode and decode message") {
			auto encodedMessage = protocol->encode(message);
			protocol::PACKET_BUFFER recievedMessage;

			for (int i = 0; i < 1024; i++)
				recievedMessage[i] = encodedMessage[i];

			auto headerType = protocol->getMessageType(recievedMessage);

			REQUIRE(headerType == protocol::CALL);

			auto decodedMessage = protocol->decodeCallMessage(recievedMessage);

			REQUIRE(decodedMessage.headerId == protocol::REQUEST_CALL);
			REQUIRE(std::string(decodedMessage.clientName) == "Foo");
			REQUIRE(std::string(decodedMessage.contactName) == "Bar");
		}
	}
}

TEST_CASE("Protocol server message encode/decode", "[protocol]")
{
	auto protocol = new protocol::Protocol();
	protocol::serverMessage message;

	SECTION("Setting up message") {
		message.headerId = protocol::SERVER_RESPONSE;
		message.response = static_cast<char>(protocol::UNAVAILABLE);
		strcpy(message.ip, "101.10.101.10");
		message.port = 6969;

		REQUIRE(std::string(message.ip) == "101.10.101.10");

		SECTION("Encode and decode message") {
			auto encodedMessage = protocol->encode(message);
			protocol::PACKET_BUFFER recievedMessage;

			for (int i = 0; i < 1024; i++)
				recievedMessage[i] = encodedMessage[i];

			auto headerType = protocol->getMessageType(recievedMessage);

			REQUIRE(headerType == protocol::SERVER);

			auto decodedMessage = protocol->decodeServerMessage(recievedMessage);

			REQUIRE(decodedMessage.headerId == protocol::SERVER_RESPONSE);
			REQUIRE(decodedMessage.response == static_cast<char>(protocol::UNAVAILABLE));
			REQUIRE(std::string(decodedMessage.ip) == "101.10.101.10");
			REQUIRE(decodedMessage.port == 6969);
		}
	}
}
