#include "Camera.h"

namespace sbe {

//Orthographic matrix initialized to identity matrix and scale to 1
Camera::Camera() :
	_isUpdateRequired(true),
	_screenDimensions(250, 250),
	_position(0.0f, 0.0f),
	_scale(1.0f),
	_cameraMatrix(1.0f),
	_orthoMatrix(1.0f) {

}

Camera::~Camera() {

}

void Camera::init(int screenWidth, int screenHeight) {
	_screenDimensions = glm::vec2((float)screenWidth, (float)screenHeight);
	_orthoMatrix = glm::ortho(0.0f, _screenDimensions.x, 0.0f, _screenDimensions.y);
}

void Camera::update() {
	//If the scale or position of camera has changed it is updated
	if (_isUpdateRequired) {
		//Generate new camera matrix by translating by the position
		glm::vec3 translate(-_position.x, -_position.y, 0.0f);
		_cameraMatrix = glm::translate(_orthoMatrix, translate);
		//And then scaling it by the scale matrix
		glm::vec3 scale(_scale, _scale, 0.0f);
		_cameraMatrix = glm::scale(_cameraMatrix, scale);
		_isUpdateRequired = false;
	}
}

glm::vec2 Camera::getWorldFromScreen(glm::vec2 screenCoordinates) {
	//Invert y direction
	screenCoordinates.y = _screenDimensions.y - screenCoordinates.y;
	//Make 0 the centre
	screenCoordinates -= glm::vec2(_screenDimensions.x / 2, _screenDimensions.y / 2);
	//Scale the coordinates
	screenCoordinates /= _scale;
	//Translate by position of camera
	screenCoordinates += _position;
	return screenCoordinates;
}

//Sets the position of the camera
void Camera::setPosition(const glm::vec2& newPostion) {
	_position = newPostion;
	_isUpdateRequired = true;
}


//Sets the scale (how zoomed in) camera is
void Camera::setScale(float newScale) {
	_scale = newScale;
	_isUpdateRequired = true;
}

//Returns position of camera
glm::vec2 Camera::getPosition() {
	return _position;
}

//Returns the scale of the camera
float Camera::getScale() {
	return _scale;
}

//Return camera matrix that every coordinate is mapped by
glm::mat4 Camera::getCameraMatrix() {
	return _cameraMatrix;
}

}