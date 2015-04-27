#include "GameObject.h"
#include <iostream>


GameObject::GameObject()
{
	int x = 0.0f;
	int y = 3.0f;
	int z = 0.0f;
	int w = 1.0f;
	int d = 1.0f;
	billbordTile = new Tile(w, d, x, y, z);
	billbordTile->LoadTexture("tiles.png");
	billbordTile->vertices[0].point = glm::vec3(x, y, z);
	billbordTile->vertices[1].point = glm::vec3(x + w, y, z);
	billbordTile->vertices[2].point = glm::vec3(x + w, y + d, z);
	billbordTile->vertices[3].point = glm::vec3(x, y + d, z);
	billbordTile->SetTileType(Grass);

	collisionBox.max[0] = 1;
	collisionBox.max[1] = 1;
	collisionBox.min[0] = 0;
	collisionBox.min[1] = 0;
}


GameObject::~GameObject()
{
}

void GameObject::Draw()
{
	billbordTile->Draw();
}

void GameObject::Update()
{

}

int GameObject::SetPosition(lua_State * L)
{
	int n = lua_gettop(L);
	if (n != 3)
	{
		std::cerr << "Expected 3 argument, " << n << " arguments found." << std::endl;
		return 0;
	}
	float x = lua_tonumber(L, -1);
	float y = lua_tonumber(L, -2);
	float z = lua_tonumber(L, -3);
	SetPosition(x, y, z);
	return 0;
}

void GameObject::SetPosition(float x, float y, float z)
{
	float w = 1.0f;
	float d = 1.0f;
	billbordTile->vertices[0].point = glm::vec3(x, y, z);
	billbordTile->vertices[1].point = glm::vec3(x + w, y, z);
	billbordTile->vertices[2].point = glm::vec3(x + w, y + d, z);
	billbordTile->vertices[3].point = glm::vec3(x, y + d, z);
	billbordTile->UpdateVRAMData();
	collisionBox.SetPosition(x, y);
}

void GameObject::SetPosition(glm::vec3 position)
{
	SetPosition(position.x, position.y, position.z);
}