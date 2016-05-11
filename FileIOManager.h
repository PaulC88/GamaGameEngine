#pragma once

#include <vector>

namespace GamaGameEngine {
	class FileIOManager	{

	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

		static bool readFileToVector(std::string filePath, std::vector<std::string>& fileData);

		static void writeToFile(std::string filePath, std::vector<std::string>& fileData);
	};
}

