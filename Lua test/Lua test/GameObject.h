#pragma once
#include "Tile.h"
#include "lua.hpp"
#include "Drawable.h"
#include "AABB.h"

class GameObject : Drawable
{
	Tile * billbordTile;
	
public:
	AABB collisionBox;

	GameObject();
	~GameObject();

	void Draw();
	void Update();

	int SetPosition(lua_State * L);
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 position);
};

