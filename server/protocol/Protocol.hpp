/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Protocol
*/

#pragma once

#include <string>
#include <cstdint>

namespace protocol {

enum header {
	CONNECT,
	DISCONNECT,
	GET_CONTACTS,
	REQUEST_CALL,
	NO_REPLY,
	END_CALL,
	AUDIO,
	SERVER_RESPONSE
};

enum messageType {
	CONNECTION,
	INFO,
	CALL,
	SERVER,
	UNDEFINED
};

struct connectionMessage {
	header headerId;
	char clientName[13];
	char ip[16];
	unsigned short port;
};

struct callMessage {
	header headerId;
	char clientName[12];
	char contactName[12];
};

struct audioMessage {
	header headerId;
	unsigned short length;
	unsigned char *data;
};

struct infoMessage {
	header headerId;
};

struct serverMessage {
	header headerId;
	char response;
	char ip[16];
	unsigned short port;
};

struct infoResponseMessage {
	header headerId;
	unsigned short nextMessageLength;
	char *contactNames;
};

enum state {
	AVAILABLE,
	UNAVAILABLE
};

using UINT8 = uint8_t;
using PACKET = UINT8*;
using PACKET_BUFFER = UINT8[1024];

unsigned const NAME_LENGTH =  12;
unsigned const IP_LENGTH =  15;
unsigned const PACKET_SIZE =  1024;

class Protocol {
	public:
		Protocol();
		~Protocol();

		PACKET encode(connectionMessage&) const noexcept;
		PACKET encode(callMessage&) const noexcept;
		PACKET encode(audioMessage&) const noexcept;
		PACKET encode(infoMessage&) const noexcept;
		PACKET encode(serverMessage&) const noexcept;
		PACKET encode(infoResponseMessage&) const noexcept;

		messageType getMessageType(PACKET_BUFFER&) const noexcept;
		connectionMessage decodeConnectionMessage(PACKET_BUFFER&) const noexcept;
		callMessage decodeCallMessage(PACKET_BUFFER&) const noexcept;
		audioMessage decodeAudioMessage(PACKET_BUFFER&) const noexcept;
		infoMessage decodeInfoMessage(PACKET_BUFFER&) const noexcept;
		serverMessage decodeServerMessage(PACKET_BUFFER&) const noexcept;
		infoResponseMessage decodeInfoResponseMessage(UINT8*, int) const noexcept;

	private:
		UINT8 *encodeHeader(UINT8*, header&) const noexcept;
		UINT8 *encodeChar(UINT8*, char&) const noexcept;
		UINT8 *encodeUChar(UINT8*, unsigned char&) const noexcept;
		UINT8 *encodeCharArray(UINT8*, char*, char) const noexcept;
		UINT8 *encodeUCharArray(UINT8*, unsigned char*, char) const noexcept;
		UINT8 *encodeUShort(UINT8*, unsigned short&) const noexcept;

		UINT8 *decodeHeader(UINT8*, header*) const noexcept;
		UINT8 *decodeChar(UINT8*, char*) const noexcept;
		UINT8 *decodeUChar(UINT8*, unsigned char*) const noexcept;
		UINT8 *decodeCharArray(UINT8*, char*, char) const noexcept;
		UINT8 *decodeUCharArray(UINT8*, unsigned char*, char) const noexcept;
		UINT8 *decodeUShort(UINT8*, unsigned short*) const noexcept;
};

}
