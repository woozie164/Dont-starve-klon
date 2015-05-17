#pragma once
#include <unordered_map>
#include <string>
#include "GL\glew.h"
#include "src\SOIL.h"

class TextureManager
{
	std::unordered_map<std::string, GLuint> texturesHandles;
public:
	TextureManager();
	~TextureManager();

	GLuint LoadTexture(std::string filename);
};

