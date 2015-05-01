#pragma once
#include "GL\glew.h"
#include "glfw3.h"
#include "glm\glm.hpp"
#include "vertex.h"
#include "Drawable.h"
#include <vector>
#include "AABB.h"
#include "Serializable.h"


enum TileType
{
	Grass,
	Rock,
	Dirt
};

class Tile : Drawable, Serializable
{
	float width, depth;
	GLuint VBO, VAO, elementVBO;
	static GLuint texture;
	TileType type;

	std::vector<unsigned int> indices;
	bool VRAMDataflag; // Set to true when the VRAM data needs to update.
	void InitBuffers();
public:
	std::vector<VertexData> vertices;
	AABB collisionBox;

	Tile();
	~Tile();

	Tile(float width, float depth, float x, float y, float z);

	void LoadTexture(const char * filename);
	void Draw();
	
	void SetPosition(float x, float y, float z);
	void SetTileType(TileType type);	
	void UpdateVRAMData();

	void Serialize(std::ostream & s);
};

