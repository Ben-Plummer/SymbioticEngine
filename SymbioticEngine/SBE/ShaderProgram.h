#pragma once
#include <GL/glew.h>
#include <string>

namespace sbe {

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void compileShadersContents(const char* vertexShaderContents, const char* fragmentShaderContents);
	void linkShaders();
	void addAttribute(const std::string& attributeName);
	GLint getUniformLocation(const std::string uniformName);

	void startUsing();
	void stopUsing();

private:
	void compileShader(const char* shaderContents, const std::string& name, GLuint id);

	int _numOfAttributes;
	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};

}