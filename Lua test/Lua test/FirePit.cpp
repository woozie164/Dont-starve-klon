#include "FirePit.h"


FirePit::FirePit()
{
	float x = 4.0f;
	float y = 3.1f;
	float z = 4.0f;
	float w = 1.0f;
	float d = 1.0f;
	tile = new Tile(w, d, x, y, z);
	tile->LoadTexture("fire_pit.png");
	tile->vertices[0].texCoord = glm::vec2(0.0f, 0.0f);
	tile->vertices[1].texCoord = glm::vec2(1.0f, 0.0f);
	tile->vertices[2].texCoord = glm::vec2(1.0f, 1.0f);
	tile->vertices[3].texCoord = glm::vec2(0.0f, 1.0f);
}


FirePit::~FirePit()
{
}

void FirePit::Draw()
{
	tile->Draw();
}

