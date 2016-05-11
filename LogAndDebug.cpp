#include "LogAndDebug.h"

#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <time.h>
#include <sstream>

namespace GamaGameEngine {

	void logData(std::string log) {

		static time_t seconds = time(0);

		std::stringstream ss;
		ss << seconds;
		static std::string ts = ss.str();
		
		std::ofstream outFile;

		//Log Directory must exist for log file to work
		outFile.open("Log/logFile-" + ts + ".txt", std::ios::app);
		outFile << log << std::endl;
	}

	void logPerfResults(std::string log) {

		static time_t seconds = time(0);

		std::stringstream ss;
		ss << seconds;
		static std::string ts = ss.str();

		std::ofstream outFile;

		//Log Directory must exist for log file to work
		outFile.open("Log/PerfResults-" + ts + ".txt", std::ios::app);
		outFile << log << std::endl;
	}

	//Prints error message to console and exits the game
	void fatalError(std::string errorString) {
		logData(errorString);
		std::cout << errorString << std::endl;
		std::cout << "Press any key to quit";
		char tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}
}