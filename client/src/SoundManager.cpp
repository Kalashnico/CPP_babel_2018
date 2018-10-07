/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Sound Manager
*/

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
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = NUM_CHANNELS;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
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

	while(Pa_IsStreamActive(stream))
		Pa_Sleep(1000);


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

	unsigned char *in = (unsigned char*) inputBuffer;
	unsigned char *out = (unsigned char*) outputBuffer;

	if (in != nullptr) {
		//auto encodedSound = _codecManager->encode(in);
		//_udpClient->sendAudioDatagram(encodedSound.encoded.data(), encodedSound.bytes);
		_udpClient->sendAudioDatagram(in, framesPerBuffer);
	}

	auto audioMessage = _udpClient->readPendingAudioDatagrams(framesPerBuffer);
	if (audioMessage.length != 0) {
		/*EncodedSound encodedSound;

		for (int i = 0; i < audioMessage.length; i++)
			encodedSound.encoded.emplace_back(audioMessage.data[i]);

		encodedSound.bytes = audioMessage.length;

		delete audioMessage.data;

		auto decodedSound = _codecManager->decode(encodedSound);*/

		for (int i = 0; i < framesPerBuffer; i++)
			out[i] = audioMessage.data[i];

		//delete decodedSound;
	} else {
		for (int i = 0; i < framesPerBuffer; i++)
			out[i] = 0;
	}

	return paContinue;
}
