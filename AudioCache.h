#pragma once
#include <map>

#include "DataTypes.h"

namespace GamaGameEngine {

	class AudioCache {

	public:
		AudioCache();
		~AudioCache();

		ALWave getWaveFile(const char* wavePath);
		ALOgg getOggFile(const char* oggPath);

		bool removeWave(const char* wavePath);
		bool removeOgg(const char* oggPath);


	private:
		std::map<const char*, ALWave> m_waveMap;
		std::map<const char*, ALOgg> m_oggMap;
	};

}