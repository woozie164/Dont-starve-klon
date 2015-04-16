#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "GL\glew.h"
#include "glfw3.h"

struct ShaderInfo
{
	std::string source;
	GLenum shaderType;
	GLuint shaderHandle;
};

class ShaderProgramManager
{
private:
	std::unordered_map<std::string, GLuint> shaderPrograms;	
	std::vector<ShaderInfo> shaders;

public:
	ShaderProgramManager() {}
	~ShaderProgramManager() {}

	void LoadShaderFromFile(std::string filename, GLenum shaderType);
	GLuint CompileShaderProgram(std::string programName);

	GLuint GetShaderProgram(std::string programName) { return shaderPrograms[programName]; }
};


void LoadShaderFromFile(std::string filename, GLenum shaderType, std::vector<ShaderInfo> & shaders);