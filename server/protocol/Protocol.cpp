/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Protocol
*/

#include <cstring>
#include "Protocol.hpp"

namespace protocol {

Protocol::Protocol()
{}

Protocol::~Protocol()
{}

PACKET Protocol::encode(connectionMessage &message) const noexcept
{
	PACKET packet = new PACKET_BUFFER;
	auto ptrBuffer = &packet[0];

	ptrBuffer = encodeHeader(ptrBuffer, message.headerId);
	ptrBuffer = encodeCharArray(ptrBuffer, message.clientName, NAME_LENGTH);
	ptrBuffer = encodeCharArray(ptrBuffer, message.ip, IP_LENGTH);
	ptrBuffer = encodeUShort(ptrBuffer, message.port);

	return packet;
}

PACKET Protocol::encode(callMessage &message) const noexcept
{
	PACKET packet = new PACKET_BUFFER;
	auto ptrBuffer = &packet[0];

	ptrBuffer = encodeHeader(ptrBuffer, message.headerId);
	ptrBuffer = encodeCharArray(ptrBuffer, message.clientName, NAME_LENGTH);
	ptrBuffer = encodeCharArray(ptrBuffer, message.contactName, NAME_LENGTH);

	return packet;
}

PACKET Protocol::encode(serverMessage &message) const noexcept
{
	PACKET packet = new PACKET_BUFFER;
	auto ptrBuffer = &packet[0];

	ptrBuffer = encodeHeader(ptrBuffer, message.headerId);
	ptrBuffer = encodeChar(ptrBuffer, message.response);
	ptrBuffer = encodeCharArray(ptrBuffer, message.ip, IP_LENGTH);
	ptrBuffer = encodeUShort(ptrBuffer, message.port);

	return packet;
}

messageType Protocol::getMessageType(PACKET_BUFFER &buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	char headerValue;
	messageType type = UNDEFINED;

	ptrBuffer = decodeChar(buffer, &headerValue);

	if (headerValue == 0 || headerValue == 1)
		type = CONNECTION;
	else if (headerValue >= 2 && headerValue <= 4)
		type = CALL;
	else if (headerValue == 5)
		type = SERVER;

	return type;
}

connectionMessage Protocol::decodeConnectionMessage(PACKET_BUFFER &buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	connectionMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);
	ptrBuffer = decodeCharArray(ptrBuffer, message.clientName, NAME_LENGTH);
	ptrBuffer = decodeCharArray(ptrBuffer, message.ip, IP_LENGTH);
	ptrBuffer = decodeUShort(ptrBuffer, &message.port);

	return message;
}

callMessage Protocol::decodeCallMessage(PACKET_BUFFER &buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	callMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);
	ptrBuffer = decodeCharArray(ptrBuffer, message.clientName, NAME_LENGTH);
	ptrBuffer = decodeCharArray(ptrBuffer, message.contactName, NAME_LENGTH);

	return message;
}

serverMessage Protocol::decodeServerMessage(PACKET_BUFFER &buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	serverMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);
	ptrBuffer = decodeChar(ptrBuffer, &message.response);
	ptrBuffer = decodeCharArray(ptrBuffer, message.ip, IP_LENGTH);
	ptrBuffer = decodeUShort(ptrBuffer, &message.port);

	return message;
}

UINT8 *Protocol::encodeHeader(UINT8 *buffer, header &value) const noexcept
{
	auto headerValue = static_cast<char>(value);
	return encodeChar(buffer, headerValue);
}

UINT8 *Protocol::encodeChar(UINT8 *buffer, char &value) const noexcept
{
	buffer[0] = value;
	return buffer + 1;
}

UINT8 *Protocol::encodeCharArray(UINT8 *buffer, char *array, char length) const noexcept
{
	for (int i = 0; i < length; i++)
		buffer = encodeChar(buffer, array[i]);
	return buffer;
}

UINT8 *Protocol::encodeUShort(UINT8 *buffer, unsigned short &value) const noexcept
{
	buffer[1] = value;
	buffer[0] = value >> 8;
	return buffer + 2;
}

UINT8 *Protocol::decodeHeader(UINT8 *buffer, header *value) const noexcept
{
	*value = static_cast<header>(buffer[0]);
	return buffer + 1;
}

UINT8 *Protocol::decodeChar(UINT8 *buffer, char *value) const noexcept
{
	*value = buffer[0];
	return buffer + 1;
}

UINT8 *Protocol::decodeCharArray(UINT8 *buffer, char *array, char length) const noexcept
{
	for (int i = 0; i < length; i++)
		buffer = decodeChar(buffer, &array[i]);
	return buffer;
}

UINT8 *Protocol::decodeUShort(UINT8 *buffer, unsigned short *value) const noexcept
{
	*value = (*value << 8) + buffer[0];
	*value = (*value << 8) + buffer[1];
	return buffer + 2;
}

}
