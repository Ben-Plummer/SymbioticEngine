#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace sbe {

TextureCache::TextureCache() {

}

TextureCache::~TextureCache() {

}

GLTexture TextureCache::getTexture(std::string textureFilePath) {
	//Scans map to find an element with key equal to textureFilePath
	auto mit = _textureMap.find(textureFilePath);

	//Check whether it is in the map
	if (mit == _textureMap.end()) {
		//Loads texture into tmpTexture
		GLTexture tmpTexture = imgloader::loadPNG(textureFilePath);

		//Inserts new texture into map
		_textureMap.insert(make_pair(textureFilePath, tmpTexture));

		std::cout << "Loaded texture!\n";
		return tmpTexture;
	}
	std::cout << "Used cached texture!\n";
	//We not that mit is now pointing to our desired texture, we return the texture the second value
	return mit->second;

}

}