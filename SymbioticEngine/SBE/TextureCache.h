#pragma once
#include <map>
#include "GLTexture.h"

namespace sbe {

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture getTexture(std::string textureFilePath);

private:
	std::map<std::string, GLTexture> _textureMap;
};

}