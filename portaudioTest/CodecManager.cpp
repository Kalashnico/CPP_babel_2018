//
// Created by Nicolas Guerin on 04/10/2018.
//

#include "CodecManager.hpp"

CodecManager::CodecManager(int channels, int bufferSize, int sampleRate)
{
	_channels = channels;
	_bufferSize = bufferSize;
	_sampleRate = sampleRate;

	int err ;

	_encoder = opus_encoder_create(sampleRate, channels, OPUS_APPLICATION_AUDIO, &err);

	if (err != OPUS_OK)
		std::cerr << "Failed to create opus encoder" << std::endl;


	_decoder = opus_decoder_create(sampleRate, channels, &err);

	if (err != OPUS_OK)
		std::cerr << "Failed to create opus decoder" << std::endl;

}

EncodedSound CodecManager::encode(unsigned short *sound)
{
	std::vector<unsigned short> sampleBlockv(sound, sound + sizeof sound / sizeof sound[0]);

	EncodedSound encodedStruct;

	encodedStruct.encoded.resize(_bufferSize * _channels * 2);

	_bytes = opus_encode(_encoder, reinterpret_cast<opus_int16 const *>(sampleBlockv.data()),
			     _bufferSize, encodedStruct.encoded.data(), encodedStruct.encoded.size());

	if (_bytes < 0)
		std::cerr << "Failed to encode sound" << std::endl;

	return encodedStruct;
}

unsigned short *CodecManager::decode(EncodedSound sound)
{

	std::vector<unsigned short> decoded;

	decoded.resize(_bufferSize * _channels);
	opus_int32 dec_bytes = opus_decode(_decoder, sound.encoded.data(), sound.bytes,
					   reinterpret_cast<opus_int16 *>(sound.encoded.data()), _bufferSize, 0);

	if (dec_bytes < 0)
		std::cerr << "Failed to decode sound" << std::endl;

	unsigned short *decodedArray = &decoded[0];

	return decodedArray;
}

