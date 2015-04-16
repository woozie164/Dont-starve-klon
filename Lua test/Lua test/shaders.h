#pragma once
#include <string>
#include <vector>

#include "GL\glew.h"
#include "glfw3.h"

struct ShaderInfo
{
	std::string source;
	GLenum shaderType;
	GLuint shaderHandle;
};

GLuint compileShaderProgram(std::vector<ShaderInfo> & shaders);
void loadShader(std::string filename, GLenum shaderType, std::vector<ShaderInfo> & shaders);