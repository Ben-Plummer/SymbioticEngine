#pragma once
#include <GL/glew.h>
#include <string>

namespace sbe {

struct GLTexture {
	std::string filePath;
	GLuint id;
	int width, height;
};

}