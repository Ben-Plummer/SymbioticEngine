#include "Window.h"
#include "Error.h"

#include <GL/glew.h>

namespace sbe {

Window::Window() :
	_sdlWindow(nullptr),
	_glContext(0),
	_screenWidth(0),
	_screenHeight(0) {

}

Window::~Window() {
	//Deletes GL Context if present
	if (_glContext != 0) {
		SDL_GL_DeleteContext(_glContext);
	}
	//Destroys SDL Window if present
	if (_sdlWindow != nullptr) {
		SDL_DestroyWindow(_sdlWindow);
		_sdlWindow = nullptr;
	}

	//Quits SDL subsystems
	SDL_Quit();
}

void Window::create(std::string windowTitle, int screenWidth, int screenHeight, unsigned int windowFlags, bool toggleVSYNC) {
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	//Sets up flags for SDL_CreateWindow()
	unsigned int currentFlags = SDL_WINDOW_OPENGL;
	if (windowFlags & BORDERLESS) currentFlags |= SDL_WINDOW_BORDERLESS;
	if (windowFlags & FULLSCREEN) currentFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	if (windowFlags & INVISIBLE) currentFlags |= SDL_WINDOW_HIDDEN;
	if (windowFlags & RESIZABLE) currentFlags |= SDL_WINDOW_RESIZABLE;

	//Initializes SDL which allows us to use SDL calls
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL Error: %s\n", SDL_GetError());
		fatalError("SDL Failed to initialize!");
	}

	//Tells SDL to use two windows, one to draw and one to clear, prevents flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Creates window
	_sdlWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _screenWidth, _screenHeight, currentFlags);
	if (_sdlWindow == nullptr) {
		fatalError("SDL failed to create window!");
	}

	//Sets up OpenGL context allowing us to draw with OpenGL
	_glContext = SDL_GL_CreateContext(_sdlWindow);
	if (_glContext == 0) {
		fatalError("SDL Failed to create an OpenGL context!");
	}

	//Sets up extensions with GLEW (OpenGL Extensions Wrangler)
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("GLEW Failed to configure extensions!");
	}

	//Prints OpenGL version to console
	printf("***** OpenGL Version %s *****\n", glGetString(GL_VERSION));
	if (glGetString(GL_VERSION)[0] < 3) {
		fatalError("OpenGL version 3 (or higher) is required, no appropriate version was found!");
	}

	//Toggle VSYNC
	SDL_GL_SetSwapInterval(toggleVSYNC);

	//Enable alpha blending so blackgrounds are transparent
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::swapBuffer() {
	SDL_GL_SwapWindow(_sdlWindow);
}

int Window::getWidth() {
	return _screenWidth;
}

int Window::getHeight() {
	return _screenHeight;
}

}