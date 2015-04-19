#pragma once

#include "glm\glm.hpp"
#include "vertex.h"
#include <vector>

class Terrain
{
	float width, depth;
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	std::vector<float> * GenerateSquare(float x, float z, float width, float depth);
	void GenerateVertices(int rows, int columns);

public:
	Terrain() {};

	Terrain(float width, float depth);
	void LoadTerrain(float width, float depth);
	const VertexData * GetVertexData() { return vertices.data(); };
	const unsigned int * GetIndicesData() { return indices.data(); };
	int GetVertexCount() { return vertices.size(); };
	int GetIndicesCount() { return indices.size(); };
	int GetVertexSize() { return sizeof(VertexData);  };
	int GetIndicesSize() { return sizeof(unsigned int); };
};
