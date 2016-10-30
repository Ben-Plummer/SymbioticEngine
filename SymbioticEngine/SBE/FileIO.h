#pragma once
#include <string>
#include <vector>

namespace sbe {

namespace fio {

extern bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
extern bool readFileToBuffer(std::string filePath, std::string& buffer);

}

}