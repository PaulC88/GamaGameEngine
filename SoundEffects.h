#pragma once

#include "LogAndDebug.h"
#include "DataTypes.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace GamaGameEngine {

	class SoundEffects {
	public:
		SoundEffects();
		~SoundEffects();

		void init(const char* fileName);
		void play();
		void release();
		void attachBufferToSource();

	private:
		ALWave m_waveFile;
		ALuint m_seSource;
		ALuint m_seBuffer;
	};

}