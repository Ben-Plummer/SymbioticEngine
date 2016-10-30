#pragma once
#include "TextureCache.h"

namespace sbe {

class ResourceManager
{
public:
	static GLTexture getTexture(std::string textureFilePath);

private:
	static TextureCache _textureCache;
};

}