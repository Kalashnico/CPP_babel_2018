/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Data Tests
*/

#include <cstring>
#include "Tests.hpp"
#include "Data.hpp"

TEST_CASE("Connecting a cllient", "[data]")
{
	protocol::connectionMessage message;
	auto data = new data::Data();

	SECTION("Creating the client") {
		message.headerId = protocol::CONNECT;
		strcpy(message.clientName, "Cheese");
		strcpy(message.ip, "999.999.999.999");
		message.port = 6969;

		REQUIRE(std::string(message.clientName) == "Cheese");
		REQUIRE(std::string(message.ip) == "999.999.999.999");

		SECTION("Interpreting message in data class") {
			auto response = data->interpretMessage(message);

			REQUIRE(response.response == 1);
		}
	}

	delete data;
}

TEST_CASE("Calling", "[data]")
{
	protocol::connectionMessage message;
	auto data = new data::Data();

	SECTION("Creating the client") {
		message.headerId = protocol::CONNECT;
		strcpy(message.clientName, "Cheese");
		strcpy(message.ip, "999.999.999.999");
		message.port = 6969;

		REQUIRE(std::string(message.clientName) == "Cheese");
		REQUIRE(std::string(message.ip) == "999.999.999.999");

		SECTION("Interpreting message in data class") {
			auto response = data->interpretMessage(message);

			REQUIRE(response.response == 1);

			SECTION("Creating the contact") {
				message.headerId = protocol::CONNECT;
				strcpy(message.clientName, "Contact");
				strcpy(message.ip, "420.420.420.420");
				message.port = 6969;

				REQUIRE(std::string(message.clientName) == "Contact");
				REQUIRE(std::string(message.ip) == "420.420.420.420");

				SECTION("Interpreting message in data class") {
					auto response = data->interpretMessage(message);

					REQUIRE(response.response == 1);

					SECTION("Calling") {
						protocol::callMessage callRequest;
						callRequest.headerId = protocol::REQUEST_CALL;
						strcpy(callRequest.clientName, "Cheese");
						strcpy(callRequest.contactName, "Contact");

						response = data->interpretMessage(callRequest);

						REQUIRE(response.response == 1);
						REQUIRE(std::string(response.ip) == "420.420.420.420");
						REQUIRE(response.port == 6969);
					}
				}
			}
		}
	}

	delete data;
}
