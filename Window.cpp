#include "Window.h"
#include "LogAndDebug.h"

#include <string>
#include <sstream>


namespace GamaGameEngine {

	Window::Window() {
		//Empty
	}

	Window::~Window() {
		//Empty
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & HIDDEN) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Open an SDL window
		m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (m_sdlWindow == nullptr)
			fatalError("Window could not be created");
		else
			logData("SDL Window created successfully");

		//Create OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
		if (glContext == nullptr)
			fatalError("SDL_GL Context could not be created");
		else
			logData("SDL_GL Context created");

		//Initialise GLEW
		GLenum error = glewInit();
		if (error != GLEW_OK)
			fatalError("GLEW could not be initialised");
		else
			logData("GLEW has been sucessfully initialised");

		std::stringstream ss;
		ss << glGetString(GL_VENDOR);
		std::string GL = ss.str();
		logData("OpenGL Vendor: " + GL);
		ss.str(std::string());
		ss << glGetString(GL_RENDERER);
		GL = ss.str();
		logData("OpenGL Renderer: " + GL);
		ss.str(std::string());
		ss << glGetString(GL_VERSION);
		GL = ss.str();
		logData("OpenGL Version: " + GL);

		//Set default window colour to black and set alpha to 1.0f
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//Disable Vsync
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(m_sdlWindow);
	}
}