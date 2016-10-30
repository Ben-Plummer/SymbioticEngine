#pragma once
#include <GL/glew.h>
#include <string>
#include "GLTexture.h"

namespace sbe {

class Sprite
{
public:
	Sprite();
	~Sprite();

	void initialize(float x, float y, float width, float height, std::string textureFilePath);
	void draw();

private:
	float _x, _y;
	float _width, _height;

	GLuint _vboID;
	GLTexture _texture;
};

}