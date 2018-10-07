/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Sound Manager
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SoundManager.hpp"

int paCallbackWire(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
	SoundManager *sm = reinterpret_cast<SoundManager*>(userData);
	return sm->paCallback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags, userData);
}

SoundManager::SoundManager(udpclient::UdpClient *udpClient)
	: _udpClient(udpClient)
{
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream = NULL;

	_codecManager = new CodecManager(NUM_CHANNELS, FRAMES_PER_BUFFER, SAMPLE_RATE);

	Pa_Initialize();

	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = NUM_CHANNELS;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowInputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	Pa_OpenStream(
		&stream,
		&inputParameters,
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,
		paCallbackWire,
		this);

	Pa_StartStream(stream);

	while (Pa_IsStreamActive(stream));

	Pa_StopStream(stream);
	Pa_CloseStream(stream);

	Pa_Terminate();
}

SoundManager::~SoundManager()
{
	delete _codecManager;
}

int SoundManager::paCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{

	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	std::cout << "Hi" << std::endl;

	unsigned char *in = (unsigned char*) inputBuffer;
	unsigned char *out = (unsigned char*) outputBuffer;

	if (in != nullptr) {
		auto encodedSound = _codecManager->encode(in);
		_udpClient->sendAudioDatagram(encodedSound.encoded.data(), encodedSound.bytes);
	}

	std::cout << "Input done" << std::endl;

	auto audioMessage = _udpClient->readPendingAudioDatagrams(framesPerBuffer * NUM_CHANNELS);

	std::cout << "Got output" << std::endl;

	if (audioMessage.length != 0) {
		EncodedSound encodedSound;

		std::cout << "Messege length: " << audioMessage.length << std::endl;

		for (int i = 0; i < audioMessage.length; i++) {
			std::cout << "Current value: " << i << std::endl;
			encodedSound.encoded.emplace_back(audioMessage.data[i]);
		}

		std::cout << "Stored audio message into an encoded struct" << std::endl;

		encodedSound.bytes = audioMessage.length;
		auto decodedSound = _codecManager->decode(encodedSound);

		std::cout << "Decoded sound" << std::endl;

		for (int i = 0; i < framesPerBuffer * NUM_CHANNELS; i++)
			out[i] = decodedSound[i];
	}

	std::cout << "Bye" << std::endl;

	return paContinue;
}
