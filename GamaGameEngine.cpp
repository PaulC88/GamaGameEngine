#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include <sstream>

#include "GamaGameEngine.h"
#include "LogAndDebug.h"

namespace GamaGameEngine {

	int init() {
		std::stringstream ss;
		std::string SDL;

		//Initialize SDL subsystems
		if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1)) {
			ss << SDL_GetError();
			SDL = ss.str();
			fatalError("Could not initialise SDL: " + SDL);
		}
		else
			logData("SDL Video and Joystick initialised");

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}
}