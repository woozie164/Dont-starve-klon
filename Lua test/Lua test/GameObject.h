#pragma once
#include "Tile.h"
#include "lua.hpp"
#include "Drawable.h"

class GameObject : Drawable
{
	Tile * billbordTile;
public:
	GameObject();
	~GameObject();

	void Draw();
	void Update();

	int SetPosition(lua_State * L);
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 position);
};

