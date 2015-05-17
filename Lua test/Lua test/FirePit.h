#pragma once
#include "Tile.h"
#include "Drawable.h"

class FirePit : Drawable
{
	Tile * tile;
public:
	FirePit();
	~FirePit();

	void Draw();
};

