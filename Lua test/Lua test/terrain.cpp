#include "terrain.h"

using namespace std;
using namespace glm;

// Bug: vertices get wrong values when width != depth
Terrain::Terrain(float width, float depth)
{
	LoadTerrain(width, depth);	
}

void Terrain::LoadTerrain(float width, float depth)
{
	this->width = width;
	this->depth = depth;
	float numOfColVert = width + 1;
	float numOfRowVert = depth + 1;
	vertices.reserve(numOfRowVert * numOfColVert);

	float halfDepth = depth / 2;
	float halfWidth = width / 2;
	for(int i = 0; i < numOfColVert; i++)
	{
		for(int j = 0; j < numOfRowVert; j++)
		{			
			float x = (float)j - halfWidth;
			float z = (float)i - halfDepth;
			vertices.push_back(
				VertexData(
					vec3(x, 0, z),	
					vec2( (x + halfWidth) / width, (z + halfDepth) / depth )));
		}
	}
	
	// Generate indices
	indices.reserve(numOfColVert * numOfRowVert); // not an accurate estimate of how many indices are needed ...
	for(unsigned int i = 0; i < depth; i++)
	{
		unsigned int rowOffset = i * numOfColVert;
		for(unsigned int j = 0; j < width; j++)
		{
			indices.push_back(j + rowOffset);
			indices.push_back(j + 1 + rowOffset);
			indices.push_back(j + 1 + numOfColVert + rowOffset);
			
			indices.push_back(j + rowOffset);
			indices.push_back(j + numOfColVert + rowOffset);
			indices.push_back(j + numOfColVert + 1 + rowOffset);
		}
	}
}
/*
#define NEAR_ZERO powf(10, -20)
void RayVsTriangle(const vec3 rayOrigin, const  vec3 rayDirection, const vec3 p0, const vec3 p1, const vec3 p2, bool & hit, float & distanceToTriangle)
{
	hit = false;
	vec3 e1 = p1 - p0;
	vec3 e2 = p2 - p0;
	vec3 q =  cross(rayDirection, e2);
	float a = dot(e1, q);
	if(a > - NEAR_ZERO && a < NEAR_ZERO) return;
	float f = 1 / a;
	vec3 s = rayOrigin - p0;
	float u = f * dot(s, q);
	if(u < 0.0f) return;
	vec3 r = cross(s, e1);
	float v = f * dot(rayDirection, r);
	if(v < 0.0f || u + v > 1.0f) return;
	distanceToTriangle = f * dot(e2, r);
	hit = true;
}

float Terrain::GetMeshYPosition(vec3 rayOrigin)
{	
	float numOfColVert = width + 1;
	for(unsigned int i = 0; i < depth; i++)
	{
		unsigned int rowOffset = i * numOfColVert;
		for(unsigned int j = 0; j < width; j++)
		{
			bool hit;
			float distance;
			RayVsTriangle(rayOrigin, vec3(0.0f, -1.0f, 0.0f),
				vertices[j + rowOffset].point, vertices[j + 1 + rowOffset].point, vertices[j + 1 + numOfColVert + rowOffset].point,
				hit, distance);

			if(hit)
			{
				float averageY = (vertices[j + rowOffset].point[1] + vertices[j + 1 + rowOffset].point[1] + vertices[j + 1 + numOfColVert + rowOffset].point[1]) / 3.0f;
				return averageY;
			}

			RayVsTriangle(rayOrigin, vec3(0.0f, -1.0f, 0.0f),
				vertices[j + rowOffset].point, vertices[j + numOfColVert + rowOffset].point, vertices[j + numOfColVert + 1 + rowOffset].point,
				hit, distance);

			if(hit)
			{
				float averageY = (vertices[j + rowOffset].point[1] + vertices[j + numOfColVert + rowOffset].point[1] + vertices[j + numOfColVert + 1 + rowOffset].point[1]) / 3.0f;
				return averageY;
			}
		}
	}
	return 0.0f;	
	}*/