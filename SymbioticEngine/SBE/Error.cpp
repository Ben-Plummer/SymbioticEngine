#include "Error.h"
#include <SDL/SDL.h>
#include <iostream>

namespace sbe {

void fatalError(std::string errorString) {
	std::cout << "Error: " << errorString << std::endl;
	if (SDL_GetError()[0] != '\0') {
		printf("SDL Error: %s\n", SDL_GetError());
	}
	SDL_Quit();
	std::cin.ignore();
	exit(7);
}

}