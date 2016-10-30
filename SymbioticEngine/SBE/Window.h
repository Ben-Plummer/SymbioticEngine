#pragma once
#include <SDL/SDL.h>
#include <string>

namespace sbe {

enum WindowFlags { BORDERLESS = 0x1, FULLSCREEN = 0x2, INVISIBLE = 0x4, RESIZABLE = 0x8};

class Window
{
public:
	Window();
	~Window();

	void create(std::string windowTitle, int screenWidth, int screenHeight, unsigned int windowFlags, bool toggleVSYNC = false);
	void swapBuffer();
	
	int getWidth();
	int getHeight();

private:
	SDL_Window* _sdlWindow;
	SDL_GLContext _glContext;
	int _screenWidth, _screenHeight;
};

}