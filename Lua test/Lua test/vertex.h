#pragma once

#include "glm/glm.hpp"

struct VertexData
{
	glm::vec3 point;
	glm::vec2 texCoord;

	VertexData()
	{

	}

	VertexData(glm::vec3 point, glm::vec2 texCoord)
	{
		this->point = point;
		this->texCoord = texCoord;
	}
};