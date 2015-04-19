#pragma once
#include "GL\glew.h"
#include "glfw3.h"
#include "glm\glm.hpp"
#include "vertex.h"
#include <vector>

class Terrain
{
	float width, depth;
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	GLuint terrainVBO;
	GLuint terrainVAO;
	GLuint terrainElementVBO;

	void InitBuffers();
public:
	Terrain() {};

	Terrain(float width, float depth);

	void LoadTerrain(float width, float depth);
	void Terrain::Draw();

	const VertexData * GetVertexData() { return vertices.data(); };
	const unsigned int * GetIndicesData() { return indices.data(); };
	int GetVertexCount() { return vertices.size(); };
	int GetIndicesCount() { return indices.size(); };
	int GetVertexSize() { return sizeof(VertexData);  };
	int GetIndicesSize() { return sizeof(unsigned int); };
};
