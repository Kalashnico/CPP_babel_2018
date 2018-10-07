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

PACKET Protocol::encode(audioMessage &message) const noexcept
{
	PACKET packet = new PACKET_BUFFER;
	auto ptrBuffer = &packet[0];

	ptrBuffer = encodeHeader(ptrBuffer, message.headerId);
	ptrBuffer = encodeUShort(ptrBuffer, message.length);
	ptrBuffer = encodeUCharArray(ptrBuffer, message.data, message.length);

	return packet;
}

PACKET Protocol::encode(infoMessage &message) const noexcept
{
	PACKET packet = new PACKET_BUFFER;
	auto ptrBuffer = &packet[0];

	ptrBuffer = encodeHeader(ptrBuffer, message.headerId);

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

PACKET Protocol::encode(infoResponseMessage &message) const noexcept
{
	PACKET packet = new PACKET_BUFFER;
	auto ptrBuffer = &packet[0];

	ptrBuffer = encodeHeader(ptrBuffer, message.headerId);
	ptrBuffer = encodeUShort(ptrBuffer, message.nextMessageLength);
	ptrBuffer = encodeCharArray(ptrBuffer, message.contactNames, strlen(message.contactNames));

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
	else if (headerValue == 2)
		type = INFO;
	else if (headerValue >= 3 && headerValue <= 6)
		type = CALL;
	else if (headerValue == 7)
		type = SERVER;

	return type;
}

connectionMessage Protocol::decodeConnectionMessage(PACKET_BUFFER &buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	connectionMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);
	ptrBuffer = decodeCharArray(ptrBuffer, message.clientName, NAME_LENGTH);
	message.clientName[12] = 0;
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

audioMessage Protocol::decodeAudioMessage(UINT8 *buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	audioMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);
	ptrBuffer = decodeUShort(ptrBuffer, &message.length);
	message.data = new unsigned char[message.length];
	ptrBuffer = decodeUCharArray(ptrBuffer, message.data, message.length);

	return message;
}

infoMessage Protocol::decodeInfoMessage(PACKET_BUFFER &buffer) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	infoMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);

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

infoResponseMessage Protocol::decodeInfoResponseMessage(UINT8 *buffer, int length) const noexcept
{
	UINT8 *ptrBuffer = &buffer[0];
	infoResponseMessage message;

	ptrBuffer = decodeHeader(ptrBuffer, &message.headerId);
	ptrBuffer = decodeUShort(ptrBuffer, &message.nextMessageLength);
	if (length > 0) {
		message.contactNames = new char[length + 1];
		ptrBuffer = decodeCharArray(ptrBuffer, message.contactNames, length);
		message.contactNames[length] = 0;
	} else
		message.contactNames = strdup("");

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

UINT8 *Protocol::encodeUChar(UINT8 *buffer, unsigned char &value) const noexcept
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

UINT8 *Protocol::encodeUCharArray(UINT8 *buffer, unsigned char *array, char length) const noexcept
{
	for (int i = 0; i < length; i++)
		buffer = encodeUChar(buffer, array[i]);
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

UINT8 *Protocol::decodeUChar(UINT8 *buffer, unsigned char *value) const noexcept
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

UINT8 *Protocol::decodeUCharArray(UINT8 *buffer, unsigned char *array, char length) const noexcept
{
	for (int i = 0; i < length; i++)
		buffer = decodeUChar(buffer, &array[i]);
	return buffer;
}

UINT8 *Protocol::decodeUShort(UINT8 *buffer, unsigned short *value) const noexcept
{
	*value = (*value << 8) + buffer[0];
	*value = (*value << 8) + buffer[1];
	return buffer + 2;
}

}
