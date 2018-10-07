/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Sound manager
*/

#pragma once

#include "UdpClient.hpp"
#include "CodecManager.hpp"
#include "portaudio.h"

#define SAMPLE_RATE  (48000)
#define FRAMES_PER_BUFFER (20 * SAMPLE_RATE / 1000)
#define NUM_CHANNELS    (1)

#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SIZE sizeof(short)
#define SAMPLE_SILENCE  (0)

class SoundManager {
	public:
		SoundManager(udpclient::UdpClient *);
		~SoundManager();

		int paCallback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
	private:
		udpclient::UdpClient *_udpClient;
		CodecManager *_codecManager;

};
