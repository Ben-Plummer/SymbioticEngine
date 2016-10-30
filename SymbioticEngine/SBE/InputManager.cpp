#include "InputManager.h"

namespace sbe {

InputManager::InputManager() :
	_mouseCoordinates(0.0f) {

}

void InputManager::update() {
	//Loop through the map of keys and copy them to _previousKeyMap for comparison
	for (auto& it : _keyMap) {
		_lastKeyMap[it.first] = it.second;
	}
}

void InputManager::pressKey(unsigned int keyID) {
	//Set a value with key equal to keyID passed in to true, if no such element exists, it is added
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	//Set a value with key equal to keyID to false
	_keyMap[keyID] = false;
}

void InputManager::setMouseCoordinates(float x, float y) {
	_mouseCoordinates.x = x;
	_mouseCoordinates.y = y;
}

bool InputManager::isKeyDown(unsigned int keyID) {
	//Create an iterator and use find() to make equal to the element, if no element exists it is equal to the end()
	auto it = _keyMap.find(keyID);
	if (it != _keyMap.end()) {
		//If found returns bool
		return it->second;
	}
	else {
		//Otherwise returns false
		return false;
	}
}

bool InputManager::isKeyPressed(unsigned int keyID) {
	//Check whether it was pressed since the last frame
	if ((isKeyDown(keyID) == true) && (wasKeyDown(keyID) == false)) {
		return true;
	}
	return false;
}

glm::vec2 InputManager::getMouseCoordinates() {
	return _mouseCoordinates;
}

bool InputManager::wasKeyDown(unsigned int keyID) {
	//Same as isKeyDown except uses lastKeyMap instead of the current one
	auto it = _lastKeyMap.find(keyID);
	if (it != _lastKeyMap.end()) {
		return it->second;
	}
	else {
		return false;
	}
}

}