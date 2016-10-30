#include "ImageLoader.h"
#include "picoPNG.h"
#include "FileIO.h"
#include "Error.h"

namespace sbe {

namespace imgloader {

GLTexture loadPNG(std::string textureFilePath) {
	//Create a GLTexture and initialize values to 0
	GLTexture finalTexture = {};

	//This is for the raw data that goes into decodePNG
	std::vector<unsigned char> rawImageData;
	//This is for the output data from decodePNG that will contain information about each pixel
	std::vector<unsigned char> finalImageData;

	//Other parameters for decodePNG
	unsigned long width, height;

	//Read the image data into a buffer
	if (fio::readFileToBuffer(textureFilePath, rawImageData) == false) {
		fatalError("Failed to load PNG file from directory " + textureFilePath + " to buffer!");
	}

	//Decode the png file into the finalImageData vector that will contain an array of pixels
	int decodeResultError = decodePNG(finalImageData, width, height, &(rawImageData[0]), rawImageData.size());
	if (decodeResultError != 0) {
		fatalError("decodePNG failed with error: " + std::to_string(decodeResultError));
	}

	//Generate an OpenGL texture object and store its id in our finalTexture GLTexture struct
	glGenTextures(1, &(finalTexture.id));

	//Bind texture
	glBindTexture(GL_TEXTURE_2D, finalTexture.id);

	//Upload pixel data to the OpenGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(finalImageData[0]));

	//Set up texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Store information in GLTexture that will be returned
	finalTexture.width = width;
	finalTexture.height = height;
	finalTexture.filePath = textureFilePath;

	return finalTexture;
}

}

}