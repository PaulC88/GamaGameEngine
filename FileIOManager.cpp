#include "FileIOManager.h"
#include "LogAndDebug.h"

#include <fstream>
#include <iostream>
#include <iterator>

namespace GamaGameEngine {

	bool FileIOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {

		std::ifstream file(filePath, std::ifstream::binary);

		if (file.fail()) {
			GamaGameEngine::fatalError("Failed to open " + filePath);
			return false;
		}

		file.seekg(0, file.end);

		unsigned int fileSize = (unsigned int)file.tellg();
		file.seekg(0, file.beg);

		fileSize -= (unsigned int)file.tellg();

		buffer.resize(fileSize);

		logData("Reading file: " + filePath);

		if (file) 
			logData("File read successfully: " + filePath);
		else 
			fatalError("Error reading file: " + filePath);

		file.read((char *)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

	bool FileIOManager::readFileToVector(std::string filePath, std::vector<std::string>& fileData) {

		std::ifstream file;
		file.open(filePath);

		if (file.fail()) {
			GamaGameEngine::fatalError("Failed to open " + filePath);
			perror(filePath.c_str());
			return false;
		}

		std::string tmp;

		while (std::getline(file, tmp)) {
			fileData.push_back(tmp);
		}

		file.close();
		return true;
	}

	void FileIOManager::writeToFile(std::string filePath, std::vector<std::string>& fileData) {
		std::ofstream newFile(filePath);
		std::ostream_iterator<std::string> outputIterator(newFile, "\n");
		std::copy(fileData.begin(), fileData.end(), outputIterator);
	}
}