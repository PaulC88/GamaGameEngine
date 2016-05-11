#include "ImageLoader.h"
#include "picoPNG.h"
#include "FileIOManager.h"
#include "LogAndDebug.h"
#include <iostream>

namespace GamaGameEngine {

	GLTexture ImageLoader::loadPNG(std::string filePath) {

		GLTexture texture = {};

		std::vector<unsigned char> input;
		std::vector<unsigned char > output;

		unsigned long width, height;

		if (FileIOManager::readFileToBuffer(filePath, input) == false)
			fatalError("Failed to load PNG files to buffer");
		else
			logData("Successfully loaded PNG files to buffer: " + filePath);
		
		int errorCode = decodePNG(output, width, height, &input[0], input.size());
		if (errorCode != 0)
			fatalError("decodePNG failed with error: " + std::to_string(errorCode));
		else
			logData("decodePNG succeded!");

		glGenTextures(1, &(texture.id));

		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(output[0]));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.width = width;
		texture.height = height;

		return texture;
	}
}