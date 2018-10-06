#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"
#include "CodecManager.hpp"

#define SAMPLE_RATE  (16000)
#define FRAMES_PER_BUFFER (20 * SAMPLE_RATE / 1000)
#define NUM_CHANNELS    (1)

#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SIZE (16)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"

CodecManager *_codecManager;

static int paCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{

	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	unsigned char *in = (unsigned char*) inputBuffer;
	unsigned char *out = (unsigned char*) outputBuffer;

	auto encodedSound = _codecManager->encode(in);
	auto decodedSound = _codecManager->decode(encodedSound);

	for (unsigned long i = 0; i < framesPerBuffer; i++)
		out[i] = in[i];

	return paContinue;
}

int main(void)
{
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream = NULL;

	_codecManager = new CodecManager(NUM_CHANNELS, FRAMES_PER_BUFFER, SAMPLE_RATE);

	Pa_Initialize();


	inputParameters.device = Pa_GetDefaultInputDevice();
	printf( "Input device # %d.\n", inputParameters.device );
	printf( "Input LL: %g s\n", Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency );
	printf( "Input HL: %g s\n", Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency );
	inputParameters.channelCount = NUM_CHANNELS;
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency ;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice();
	printf( "Output device # %d.\n", outputParameters.device );
	printf( "Output LL: %g s\n", Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency );
	printf( "Output HL: %g s\n", Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency );
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
		paCallback,
		NULL);

	Pa_StartStream(stream);

	while (Pa_IsStreamActive(stream));

	Pa_StopStream(stream);
	Pa_CloseStream(stream);

	Pa_Terminate();
}