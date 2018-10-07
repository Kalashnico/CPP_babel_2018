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

	_encoder = opus_encoder_create(sampleRate, channels, OPUS_APPLICATION_VOIP, &err);

	if (err != OPUS_OK)
		std::cerr << "Failed to create opus encoder" << std::endl;


	_decoder = opus_decoder_create(sampleRate, channels, &err);

	if (err != OPUS_OK)
		std::cerr << "Failed to create opus decoder" << std::endl;

}

EncodedSound CodecManager::encode(unsigned char *sound)
{
	EncodedSound encodedStruct;
	encodedStruct.encoded.resize(_bufferSize);

	_bytes = opus_encode(_encoder, reinterpret_cast<opus_int16 const *>(sound),
			     _bufferSize, encodedStruct.encoded.data(), _bufferSize);

	if (_bytes < 0)
		std::cerr << "Failed to encode sound" << std::endl;

	encodedStruct.bytes = _bytes;

	return encodedStruct;
}

unsigned char *CodecManager::decode(EncodedSound sound)
{
	std::vector<unsigned char> decoded;

	decoded.resize(_bufferSize);
	auto dec_bytes = opus_decode(_decoder, sound.encoded.data(), sound.bytes,
					   reinterpret_cast<opus_int16 *>(decoded.data()), _bufferSize, 0);

	if (dec_bytes < 0) {
		std::cerr << "Failed to decode sound" << std::endl;
		std::fill(decoded.begin(), decoded.end(), 0);
	}

	unsigned char *decodedArray = (unsigned char*) malloc(_bufferSize);
	std::copy(decoded.begin(), decoded.end(), decodedArray);

	return decodedArray;
}

