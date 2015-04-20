#pragma once
#include "GL\glew.h"
#include "glfw3.h"
#include "glm\glm.hpp"
#include "vertex.h"
#include <vector>


enum TileType
{
	Grass,
	Rock,
	Dirt
};

class Tile
{
	GLuint VBO, VAO, elementVBO;
	GLuint texture;

	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	void InitBuffers();
public:
	TileType type;

	Tile();
	~Tile();

	Tile(float width, float depth, float x, float y, float z);

	void LoadTexture(const char * filename);
	void Draw();

	void Tile::SetTileType(TileType type);
};

