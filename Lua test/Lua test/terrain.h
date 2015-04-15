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
	const VertexData * GetData() { return vertices.data(); };
	const unsigned int * GetIndices() { return indices.data(); };
	int GetVertexCount() { return vertices.size(); };
	int GetIndicesCount() { return indices.size(); };
	float GetMeshYPosition(glm::vec3 rayOrigin);
};
