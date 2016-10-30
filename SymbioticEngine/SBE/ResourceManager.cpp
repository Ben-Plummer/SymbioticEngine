#include "ResourceManager.h"

namespace sbe {

//Variables in static classes must be redefined in the cpp file
TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::getTexture(std::string textureFilePath) {
	return _textureCache.getTexture(textureFilePath);
}

}