#define _CRT_SECURE_NO_DEPRECATE
#include "AudioLoader.h"
#include "LogAndDebug.h"

#include <vorbis/vorbisfile.h>
#include <sstream>

#define BUFFER_SIZE     32768

namespace GamaGameEngine {

	ALWave AudioLoader::loadWaveFile(const char* wavePath) {

		ALWave wave = {};

		FILE* wFile = fopen(wavePath, "rb");

		fread(wave.type, sizeof(char), 4, wFile);
		if (wave.type[0] != 'R' || wave.type[1] != 'I' || wave.type[2] != 'F' || wave.type[3] != 'F') {
			fatalError(std::string(wavePath) + " is not corect format: Missing RIFF");
		}
		fread(&wave.size, sizeof(DWORD), 1, wFile);
		fread(wave.type, sizeof(char), 4, wFile);
		if (wave.type[0] != 'W' || wave.type[1] != 'A' || wave.type[2] != 'V' || wave.type[3] != 'E') {
			fatalError(std::string(wavePath) + " is not corect format: Missing WAVE");
		}
		fread(wave.type, sizeof(char), 4, wFile);
		if (wave.type[0] != 'f' || wave.type[1] != 'm' || wave.type[2] != 't' || wave.type[3] != ' ') {
			fatalError(std::string(wavePath) + " is not corect format: Missing fmt ");
		}

		fread(&wave.chunkSize, sizeof(DWORD), 1, wFile);
		fread(&wave.formatType, sizeof(short), 1, wFile);
		fread(&wave.channels, sizeof(short), 1, wFile);
		fread(&wave.sampleRate, sizeof(DWORD), 1, wFile);
		fread(&wave.avgBytesPerSecond, sizeof(DWORD), 1, wFile);
		fread(&wave.bytesPerSample, sizeof(short), 1, wFile);
		fread(&wave.bitsPerSample, sizeof(short), 1, wFile);

		fread(wave.type, sizeof(char), 4, wFile);
		if (wave.type[0] != 'd' || wave.type[1] != 'a' || wave.type[2] != 't' || wave.type[3] != 'a')
			fatalError(std::string(wavePath) + " is not correct format: Missing data");

		fread(&wave.dataSize, sizeof(DWORD), 1, wFile);

		wave.buf = new unsigned char[wave.dataSize];
		fread(wave.buf, sizeof(BYTE), wave.dataSize, wFile);

		fclose(wFile);

		std::stringstream ss;
		ss << "Chunk Size: " << wave.chunkSize << "\nFormat Type: " << wave.formatType
			<< "\nChannels: " << wave.channels << "\nSample Rate: " << wave.sampleRate
			<< "\nAverage Bytes per Second: " << wave.avgBytesPerSecond
			<< "\nBytes Per Sample: " << wave.bytesPerSample << "\nBits per Sample"
			<< wave.bitsPerSample << "\nData size: " << wave.dataSize;

		logData(std::string(wavePath) + " information: \n" + std::string(ss.str()));
		return wave;
	}

	ALOgg AudioLoader::loadOggFile(const char* oggPath) {

		ALOgg ogg = {};

		int endian = 0;
		int bitStream;
		long bytes;
		char array[BUFFER_SIZE];
		

		FILE* oggFile = fopen(oggPath, "rb");
		if (oggFile == NULL)
			fatalError("Cannot open file for reading: " + std::string(oggPath));
		
		if (ov_open_callbacks(oggFile, &ogg.oggStream, NULL, 0, OV_CALLBACKS_DEFAULT) != 0) 
			fatalError("Error opening file for decoding: " + std::string(oggPath));
		
		ogg.pInfo = ov_info(&ogg.oggStream, -1);

		ogg.frequency = ogg.pInfo->rate;

		do {
			bytes = ov_read(&ogg.oggStream, array, BUFFER_SIZE, endian, 2, 1, &bitStream);


			if (bytes < 0) {
				ov_clear(&ogg.oggStream);
				fatalError("Error decoding: " + std::string(oggPath));
			}

			ogg.bufferData.insert(ogg.bufferData.end(), array, array + bytes);
		} while (bytes > 0);

		ov_clear(&ogg.oggStream);

		return ogg;
	}
}
