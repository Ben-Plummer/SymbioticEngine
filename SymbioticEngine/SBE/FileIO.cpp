#include "FileIO.h"
#include <fstream>

namespace sbe {

namespace fio {

bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
	//Opens file a return false if it fails
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//Go to the end of the file
	file.seekg(0, std::ios::end);

	//Work out the file size
	unsigned int fileSize = (unsigned int)file.tellg();

	//Go back to start of the file
	file.seekg(0, std::ios::beg);

	//Reduce the file by any headers that may exist
	fileSize -= (unsigned int)file.tellg();

	//Resize the buffer
	buffer.resize(fileSize);

	//Read the file
	file.read((char*)&(buffer[0]), fileSize);

	//Close the file
	file.close();

	return true;
}

bool readFileToBuffer(std::string filePath, std::string& buffer) {
	//Opens file a return false if it fails
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	//Go to the end of the file
	file.seekg(0, std::ios::end);

	//Work out the file size
	unsigned int fileSize = (unsigned int)file.tellg();

	//Go back to start of the file
	file.seekg(0, std::ios::beg);

	//Reduce the file by any headers that may exist
	fileSize -= (unsigned int)file.tellg();

	//Resize the buffer
	buffer.resize(fileSize);

	//Read the file
	file.read((char*)&(buffer[0]), fileSize);

	//Close the file
	file.close();

	return true;
}

}

}