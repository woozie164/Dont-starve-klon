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
};

