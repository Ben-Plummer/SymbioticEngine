#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace sbe {

class Camera
{
public:
	Camera();
	~Camera();
	void init(int screenWidth, int screenHeight);
	void update();
	glm::vec2 getWorldFromScreen(glm::vec2 screenCoordinates);

	void setPosition(const glm::vec2& newPostion);
	void setScale(float newScale);

	glm::vec2 getPosition();
	float getScale();
	glm::mat4 getCameraMatrix();

private:
	glm::vec2 _screenDimensions;
	bool _isUpdateRequired;
	glm::vec2 _position;
	float _scale;
	glm::mat4 _cameraMatrix;
	glm::mat4 _orthoMatrix;
};

}