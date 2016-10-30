#include "FpsLimiter.h"
#include <SDL/SDL.h>

namespace sbe {

FpsLimiter::FpsLimiter() :
	_maxFPS(60) {

}

void FpsLimiter::setMaxFPS(float maxFPS) {
	_maxFPS = maxFPS;
}

void FpsLimiter::start() {
	//SDL_GetTicks returns the number of milliseconds since SDL_Init()
	_initialTicks = SDL_GetTicks();
}

//Returns fps
float FpsLimiter::stop() {
	calculateFPS();

	//Work out how many ticks this frame by getting current ticks and subtracting the ticks at the start of the frame
	float frameTicks = (float)(SDL_GetTicks() - _initialTicks);
	//Checks that whether actual frame ticks was more than desired, if it was delays by a certain amount
	if ((1000.0f / _maxFPS) > frameTicks) {
		SDL_Delay((Uint32)(1000.0f / _maxFPS - frameTicks));
	}
	
	return _fps;
}

void FpsLimiter::calculateFPS() {
	//The amount of frames averaged
	static const int NUM_FRAMES = 10;
	//An array of frameTimes that will be averaged
	static float frameTimes[NUM_FRAMES];
	//Stores the current frame
	static int currentFrame = 0;

	//The ticks of the previous frame (starts out equal to current frame)
	static Uint32 previousTicks = SDL_GetTicks();

	//The ticks for the current frame
	Uint32 currentTicks = SDL_GetTicks();

	//Calculates the frame time of the last frame in milliseconds
	_frameTime = (float)(currentTicks - previousTicks);
	//Stores the frame time in the circular buffer of frameTimes
	frameTimes[currentFrame % NUM_FRAMES] = _frameTime;

	//Sets previousTicks to currentTicks
	previousTicks = currentTicks;

	//Keep track of how many frames we are using so we know what to divide by when finding mean
	int count;

	//Increases currentFrame by 1 each frame
	currentFrame++;

	//If the currentFrame is less than the number of frames we are averaging, set count to the currentFrame
	if (currentFrame < NUM_FRAMES) {
		count = currentFrame;
	}
	else {
		count = NUM_FRAMES;
	}

	//Average frame times in the frameTimes circular buffer
	float frameTimeAverage = 0;
	//Adds up frame times
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	//Divides them by count (which is normally set to NUM_FRAMES unless its smaller
	frameTimeAverage /= count;

	//Calculate FPS
	if (frameTimeAverage > 0) { //Make sure we don't divide by 0
		_fps = 1000.0f / frameTimeAverage;
	}
	else {
		_fps = _maxFPS;
	}
}

}