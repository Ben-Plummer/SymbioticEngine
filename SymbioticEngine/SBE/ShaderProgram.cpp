#include "ShaderProgram.h"
#include "FileIO.h"
#include "Error.h"

namespace sbe {

ShaderProgram::ShaderProgram() :
	_numOfAttributes(0),
	_programID(0),
	_vertexShaderID(0),
	_fragmentShaderID(0) {

}

ShaderProgram::~ShaderProgram() {
	//Delete shaders and program if they exist
	if (_vertexShaderID != 0) {
		glDeleteShader(_vertexShaderID);
	}
	if (_fragmentShaderID != 0) {
		glDeleteShader(_fragmentShaderID);
	}
	if (_programID != 0) {
		glDeleteProgram(_programID);
	}
}

void ShaderProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	//Create 2 buffers for the contents of the shaders
	std::string vertexShaderContents;
	std::string fragmentShaderContents;

	//Read both file into the buffers
	fio::readFileToBuffer(vertexShaderFilePath, vertexShaderContents);
	fio::readFileToBuffer(fragmentShaderFilePath, fragmentShaderContents);

	//Compile the shaders
	compileShadersContents(vertexShaderContents.c_str(), fragmentShaderContents.c_str());
}

void ShaderProgram::compileShadersContents(const char* vertexShaderContents, const char* fragmentShaderContents) {
	//Create a program object
	_programID = glCreateProgram();

	//Shader names for error handling, so console displays which shader caused error
	std::string vertexShaderName = "Vertex Shader";
	std::string fragmentShaderName = "Fragment Shader";

	//Create a vertex shader object and store its ID
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		fatalError("OpenGL failed to create a vertex shader object!");
	}

	//Create a fragment shader object and store its ID
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		fatalError("OpenGL failed to create a fragment shader object!");
	}

	//Compile each shader
	compileShader(vertexShaderContents, vertexShaderName, _vertexShaderID);
	compileShader(fragmentShaderContents, fragmentShaderName, _fragmentShaderID);
}

void ShaderProgram::linkShaders() {
	//Attach shaders to program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link program together
	glLinkProgram(_programID);

	//Get the link status of the link and store in isLinked
	GLint isLinked = GL_FALSE;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);

	//Check whether an error occurred and then handle it
	if (isLinked == GL_FALSE) {
		//Get the length of the error log and store it in errorLogLength
		GLint errorLogLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &errorLogLength);

		//Create a vector of chars the length of errorLogLength
		std::vector<char> errorLog(errorLogLength);
		glGetProgramInfoLog(_programID, errorLogLength, &errorLogLength, &errorLog[0]);

		//Print out the error log
		printf("Error: %s\n", &(errorLog[0]));

		//Delete program to prevent memory leak
		glDeleteProgram(_programID);
		//Delete shaders to prevent memory leak
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
		
		fatalError("OpenGL failed to link program!");
	}

	//Detach and delete shaders after successful link
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void ShaderProgram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(_programID, _numOfAttributes++, attributeName.c_str());
}

GLint ShaderProgram::getUniformLocation(const std::string uniformName) {
	GLint uniformLocation = glGetUniformLocation(_programID, uniformName.c_str());
	if (uniformLocation == GL_INVALID_INDEX) {
		fatalError("Uniform variable " + uniformName + " not found in shader!");
	}
	return uniformLocation;
}

void ShaderProgram::startUsing() {
	glUseProgram(_programID);
	//Enable each attribute
	for (int i = 0; i < _numOfAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void ShaderProgram::stopUsing() {
	glUseProgram(0);
	//Disable each attribute
	for (int i = 0; i < _numOfAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void ShaderProgram::compileShader(const char* shaderContents, const std::string& name, GLuint id) {
	//Tell OpenGL we want to use shaderContents as the source
	glShaderSource(id, 1, &shaderContents, nullptr);

	//Compile shader
	glCompileShader(id);

	//Get the result of the compilation and store it in isCompiled
	GLint isCompiled = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	//Check whether an error has occured and handle it
	if (isCompiled == GL_FALSE) {
		//Get the length of the error log and store it in errorLogLength
		GLint errorLogLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLogLength);

		//Create a vector of chars the length of errorLogLength
		std::vector<char> errorLog(errorLogLength);
		glGetShaderInfoLog(id, errorLogLength, &errorLogLength, &errorLog[0]);

		//Print errorLog
		printf("Error: %s\n", &(errorLog[0]));

		//Delete shader to prevent it being leaked
		glDeleteShader(id);

		fatalError("Shader " + name + " failed to compile!");
	}
}

}