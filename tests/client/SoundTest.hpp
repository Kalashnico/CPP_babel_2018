/*
** EPITECH PROJECT, 2018
** Babel
** File description:
** Sound manager
*/

#pragma once

#include "portaudio.h"

#define SAMPLE_RATE  (48000)
#define FRAMES_PER_BUFFER (960)
#define NUM_CHANNELS    (1)
#define PA_SAMPLE_TYPE  paInt16

class SoundManager {
	public:
		SoundManager();
		~SoundManager();

		int paCallback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
	private:

};
