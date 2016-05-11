#pragma once

#include "TextureCache.h"
#include "AudioCache.h"
#include <string>

namespace GamaGameEngine {

	class ResourceManager {

	public:
		static GLTexture getTexture(std::string texturePath);

		static bool removeTexture(std::string texturePath);

		static ALWave getWaveFile(const char* wavePath);

		static bool removeWave(const char* wavePath);

		static ALOgg getOggFile(const char* oggPath);

		static bool removeOgg(const char* oggPath);

	private:
		static TextureCache m_textureCache;

		static AudioCache m_waveCache;

		static AudioCache m_oggCache;
	};
}
