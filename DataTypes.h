#pragma once

#include <GL\glew.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <windows.h>
#include <vector>
#include <vorbis/vorbisfile.h>

#undef min
#undef max

namespace GamaGameEngine {

	struct GLTexture {
		GLuint id;
		int width;
		int height;
	};

	struct ALWave {
		char type[4];
		DWORD size, chunkSize;
		DWORD sampleRate, avgBytesPerSecond;
		DWORD dataSize;
		short formatType, channels;
		short bytesPerSample, bitsPerSample;
		ALuint frequency;
		ALenum format;
		unsigned char* buf;
	};

	struct ALOgg {
		ALenum format;
		ALsizei frequency;
		OggVorbis_File oggStream;
		vorbis_info* pInfo;
		std::vector<char> bufferData;
	};

	struct Position {
		float x;
		float y;
	};

	struct UV {
		float u;
		float v;
	};

	struct Colour {
		Colour() :r(0), g(0), b(0), a(0) {	}
		Colour(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :r(R), g(G), b(B), a(A) {	}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct Vertex {

		Position position;

		Colour colour;

		UV uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			colour.r = r;
			colour.g = g;
			colour.b = b;
			colour.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}