#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>
#include <vector>

#define BUFFER_SIZE     32768

namespace GamaGameEngine {
	class AudioEngine {

	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

	private:
		ALCdevice* m_device = NULL;
		ALCcontext* m_context = NULL;
		char* devices;
		const char* defaultDeviceName;

	};

}