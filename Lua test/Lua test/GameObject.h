#pragma once
#include "Tile.h"
#include "lua.hpp"

class GameObject
{
	Tile * billbordTile;
public:
	GameObject();
	~GameObject();

	void Draw();

	int SetPosition(lua_State * L);
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 position);
};

