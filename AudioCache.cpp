#include "AudioCache.h"
#include "LogandDebug.h"
#include "AudioLoader.h"

namespace GamaGameEngine {

	AudioCache::AudioCache() {
		//Empty
	}


	AudioCache::~AudioCache() {
		//Empty
	}

	ALWave AudioCache::getWaveFile(const char* wavePath) {
		auto mit = m_waveMap.find(wavePath);

		if (mit == m_waveMap.end()) {
			ALWave newWave = AudioLoader::loadWaveFile(wavePath);

			m_waveMap.insert(std::make_pair(wavePath, newWave));

			logData("Loaded WAVE file to Cache: " + std::string(wavePath));
			return newWave;
		}
		return mit->second;
	}

	bool AudioCache::removeWave(const char* wavePath) {

		auto mit = m_waveMap.find(wavePath);
		if (mit == m_waveMap.end()) {
			logData("Attempted to remove nonexistant audio file from cache: " + std::string(wavePath));
			return true;
		}
		else {
			m_waveMap.erase(wavePath);
			logData("Audio file removed from cache by Resource Manager: " + std::string(wavePath));
			return true;
		}
		return false;
	}

	ALOgg AudioCache::getOggFile(const char* oggPath) {
		auto mit = m_oggMap.find(oggPath);

		if (mit == m_oggMap.end()) {
			ALOgg newOgg = AudioLoader::loadOggFile(oggPath);

			m_oggMap.insert(std::make_pair(oggPath, newOgg));

			logData("Loaded OGG file to Cache: " + std::string(oggPath));
			return newOgg;
		}
		return mit->second;
	}

	bool AudioCache::removeOgg(const char* oggPath) {

		auto mit = m_oggMap.find(oggPath);
		if (mit == m_oggMap.end()) {
			logData("Attempted to remove nonexistant audio file from cache: " + std::string(oggPath));
			return true;
		}
		else {
			m_oggMap.erase(oggPath);
			logData("Audio file removed from cache by Resource Manager: " + std::string(oggPath));
			return true;
		}
		return false;
	}

}