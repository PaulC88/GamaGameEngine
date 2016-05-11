#pragma once

#include <string>

namespace GamaGameEngine {
	
	extern void logData(std::string log);

	extern void logPerfResults(std::string log);

	extern void fatalError(std::string errorString);
}