#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

GLuint TextureManager::LoadTexture(std::string filename)
{
	auto it = texturesHandles.find(filename);
	if (it != texturesHandles.end()) 
	{
		return texturesHandles[filename];
	}
	GLuint textureH = SOIL_load_OGL_texture
		(
			filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	if (textureH == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	else
	{
		texturesHandles[filename] = textureH;
	}
	return textureH;
}
