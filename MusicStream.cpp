#include "MusicStream.h"
#include "ResourceManager.h"


namespace GamaGameEngine {

	//Initiliase buffer and source on initialisation
	MusicStream::MusicStream() {
		alGenBuffers(1, &msBuffer);
		check();
		alGenSources(1, &msSource);
		check();

		m_ss << "OpenAL source id: " << msSource << "\n";
		m_ss << "OpenAL buffer id: " << msBuffer;
		std::string infoAL = m_ss.str();
		logData(infoAL);
	}

	//Calls the release method on deconstruction 
	MusicStream::~MusicStream() {
		release();
	}

	//Gets the audio asset from the Resource Manager
	void MusicStream::init(const char* fileName) {
		m_oggFile = ResourceManager::getOggFile(fileName);
	}

	//Deletes all Buffers and Sources
	void MusicStream::release() {

		alSourceStop(msSource);
		alDeleteSources(1, &msSource);
		check();
		alDeleteBuffers(1, &msBuffer);
		check();
		logData("MusicStream sources and buffers deleted");
	}

	//Initialise Buffer with Audio data and attach to OpenAL source
	void MusicStream::attachBufferToSource() {

		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

		alSource3f(msSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(msSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSource3f(msSource, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
		alSourcef(msSource, AL_ROLLOFF_FACTOR, 0.0f);
		alSourcei(msSource, AL_SOURCE_RELATIVE, AL_TRUE);

		if (m_oggFile.pInfo->channels == 1)
			m_oggFile.format = AL_FORMAT_MONO16;
		else
			m_oggFile.format = AL_FORMAT_STEREO16;
		
		alBufferData(msBuffer, m_oggFile.format, &m_oggFile.bufferData[0], 
			static_cast<ALsizei>(m_oggFile.bufferData.size()), m_oggFile.frequency);
		check();
		// Attach sound buffer to source
		alSourcei(msSource, AL_BUFFER, msBuffer);
		check();
		alSourcei(msSource, AL_LOOPING, 1);
		check();
	}

	//Play Music Stream
	void MusicStream::play() {
		alSourcePlay(msSource);
		check();
	}

	//Check if music is playing
	bool MusicStream::isPlaying() {
		ALenum state;
		alGetSourcei(msSource, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	//Check for OpenAL errors
	void MusicStream::check() {
		int error = alGetError();
		if (error == AL_INVALID_NAME) {
			std::cout << "Invalid Name" << std::endl;
		}
		else if (error == AL_INVALID_ENUM)
		{
			std::cout << "Invalid enum" << std::endl;
		}
		else if (error == AL_INVALID_VALUE)
		{
			std::cout << "Invalid value" << std::endl;
		}
		else if (error == AL_INVALID_OPERATION)
		{
			std::cout << "Invalid operation" << std::endl;
		}
		else if (error == AL_OUT_OF_MEMORY)
		{
			std::cout << "Invalid memory" << std::endl;
		}
		else if (error != AL_NO_ERROR) {
			throw std::string("OpenAL error was raised.");
		}
	}
}