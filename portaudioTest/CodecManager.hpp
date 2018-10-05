//
// Created by Nicolas Guerin on 04/10/2018.
//

#pragma once

#include <opus/opus.h>
#include <iostream>
#include <vector>

struct EncodedSound {
	std::vector<unsigned char> encoded;
	int bytes;
};


class CodecManager {
	public:
		CodecManager(int channels, int bufferSize, int sampleRate);
		~CodecManager() = default;
		EncodedSound encode(unsigned short *sound);
		unsigned short *decode(EncodedSound sound);

	private:
		int _channels;
		int _bufferSize;
		int _sampleRate;

		OpusEncoder *_encoder;
		OpusDecoder *_decoder;
		opus_int32  _bytes;
};
