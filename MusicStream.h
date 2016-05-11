#pragma once

#include "LogAndDebug.h"
#include "DataTypes.h"

#include <string>
#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include <iostream>
#include <sstream>

namespace GamaGameEngine {

	class MusicStream {

	public:
		friend class AudioEngine;
		MusicStream();
		~MusicStream();

		void init(const char* fileName);
		void release();
		bool isPlaying();
		void attachBufferToSource();
		void play();
		void check();

	private:

		ALOgg m_oggFile;
		ALuint msBuffer;
		ALuint msSource;
		std::stringstream m_ss;
	};

}