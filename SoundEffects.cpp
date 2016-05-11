#include "SoundEffects.h"
#include "ResourceManager.h"

#include <iostream>

namespace GamaGameEngine {

	//Initiliase buffer and source on initialisation
	SoundEffects::SoundEffects() {
		alGenBuffers(1, &m_seBuffer);
		alGenSources(1, &m_seSource);
	}

	//Calls the release method on deconstruction 
	SoundEffects::~SoundEffects() {
		release();
	}

	//Gets the audio asset from the Resource Manager
	void SoundEffects::init(const char* fileName) {
		m_waveFile = ResourceManager::getWaveFile(fileName);
	}

	//Plays the Sound Effect
	void SoundEffects::play() {
		alSourcePlay(m_seSource);
		if (alGetError() != AL_NO_ERROR)
			logData("Error playing Sound Effect");
	}

	//Deletes all Buffers and Sources
	void SoundEffects::release() {
		alSourceStop(m_seSource);
		alDeleteSources(1, &m_seSource);
		alDeleteBuffers(1, &m_seBuffer);
		delete[] m_waveFile.buf;
		logData("Sound Effect sources and buffers deleted");
	}

	//Initialise Buffer with Audio data and attach to OpenAL source
	void SoundEffects::attachBufferToSource() {

		m_waveFile.frequency = m_waveFile.sampleRate;
		m_waveFile.format = 0;

		if (m_waveFile.bitsPerSample == 8)	{
			if (m_waveFile.channels == 1)
				m_waveFile.format = AL_FORMAT_MONO8;
			else if (m_waveFile.channels == 2)
				m_waveFile.format = AL_FORMAT_STEREO8;
		}
		else if (m_waveFile.bitsPerSample == 16)	{
			if (m_waveFile.channels == 1)
				m_waveFile.format = AL_FORMAT_MONO16;
			else if (m_waveFile.channels == 2)
				m_waveFile.format = AL_FORMAT_STEREO16;
		}
		if (!m_waveFile.format) {
			std::cout << "Wrong BitPerSample";
		}
		alBufferData(m_seBuffer, m_waveFile.format, m_waveFile.buf, m_waveFile.dataSize, m_waveFile.frequency);
		if (alGetError() != AL_NO_ERROR)
			std::cout << "error loading buffer";

		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

		alSource3f(m_seSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(m_seSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSource3f(m_seSource, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
		alSourcef(m_seSource, AL_PITCH, 1.0f);
		alSourcef(m_seSource, AL_GAIN, 1.0f);

		alSourcei(m_seSource, AL_BUFFER, m_seBuffer);
		alSourcei(m_seSource, AL_LOOPING, AL_FALSE);
	}
}