#pragma once
#include "DataTypes.h"

#include <string>

namespace GamaGameEngine {

	class AudioLoader {

	public:
		static ALWave loadWaveFile(const char* wavePath);

		static ALOgg loadOggFile(const char* oggPath);

	};

}