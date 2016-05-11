#include "AudioEngine.h"
#include "LogAndDebug.h"

#include <stdlib.h>
#include <iostream>


namespace GamaGameEngine {

	AudioEngine::AudioEngine() {
		init();
	}


	AudioEngine::~AudioEngine() {
		destroy();
	}

	void AudioEngine::init() {

		if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT")) {
			devices = (char *)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
			defaultDeviceName = (char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

			logData("Device used by Audio Engine: " + std::string(defaultDeviceName));
		}

		m_device = alcOpenDevice(NULL);

		if (m_device) {
			m_context = alcCreateContext(m_device, NULL);
			
			if (m_context) {
				alcMakeContextCurrent(m_context);
				logData("OpenAL context created sucessfully");
			}
			else {
				alcCloseDevice(m_device);
				fatalError("Could not create OpenAL context");
			}
		}
	}

	void AudioEngine::destroy() {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
		logData("OpenAL context destroyed and device closed");
	}
}