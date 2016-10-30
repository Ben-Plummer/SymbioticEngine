#pragma once
#include <GLM/glm.hpp>
#include <unordered_map>

namespace sbe {

class InputManager
{
public:
	InputManager();
	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	void setMouseCoordinates(float x, float y);

	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID);
	glm::vec2 getMouseCoordinates();
	
private:
	bool wasKeyDown(unsigned int keyID);

	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _lastKeyMap;
	glm::vec2 _mouseCoordinates;
};

}