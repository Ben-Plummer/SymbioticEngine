#pragma once

namespace sbe {

class FpsLimiter
{
public:
	FpsLimiter();

	void setMaxFPS(float maxFPS);

	void start();
	float stop();

private:
	void calculateFPS();

	float _fps;
	float _maxFPS;
	float _frameTime;
	unsigned int _initialTicks;
};

}