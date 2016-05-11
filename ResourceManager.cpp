#include "ResourceManager.h"


namespace GamaGameEngine {

	TextureCache ResourceManager::m_textureCache;

	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return m_textureCache.getTexture(texturePath);
	}

	bool ResourceManager::removeTexture(std::string texturePath) {
		return m_textureCache.removeTexture(texturePath);
	}

	AudioCache ResourceManager::m_waveCache;

	ALWave ResourceManager::getWaveFile(const char* wavePath) {
		return m_waveCache.getWaveFile(wavePath);
	}

	bool ResourceManager::removeWave(const char* wavePath) {
		return m_waveCache.removeWave(wavePath);
	}

	AudioCache ResourceManager::m_oggCache;

	ALOgg ResourceManager::getOggFile(const char* oggPath) {
		return m_oggCache.getOggFile(oggPath);
	}

	bool ResourceManager::removeOgg(const char* oggPath) {
		return m_oggCache.removeOgg(oggPath);
	}
}