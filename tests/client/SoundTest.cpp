/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Sound Manager Test
*/

#include <stdio.h>
#include "SoundTest.hpp"

int paCallbackWire(const void *inputBuffer, void *outputBuffer,
			unsigned long framesPerBuffer,
			const PaStreamCallbackTimeInfo *timeInfo,
			PaStreamCallbackFlags statusFlags,
			void *userData)
{
	SoundManager *sm = reinterpret_cast<SoundManager*>(userData);
	return sm->paCallback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags, userData);
}

SoundManager::SoundManager()
{
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream = nullptr;

	Pa_Initialize();

	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
	inputParameters.hostApiSpecificStreamInfo = nullptr;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = NUM_CHANNELS;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = nullptr;

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

	getchar();

	Pa_StopStream(stream);
	Pa_CloseStream(stream);

	Pa_Terminate();
}

SoundManager::~SoundManager()
{}

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
		for (int i = 0; i < framesPerBuffer; i++) {
			*out++ = *in++;
			*out++ = *in++;
		}
	} else {
		for (int i = 0; i < framesPerBuffer; i++) {
			*out++ = 0;
			*out++ = 0;
		}
	}

	return paContinue;
}

int main()
{

	SoundManager *soundManager = new SoundManager();
	delete soundManager;

	return 0;
}
