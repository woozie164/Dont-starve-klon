#pragma once
#include "Drawable.h"

class Food : Drawable
{
	int foodValue;

public:
	Food();
	~Food();

	void Draw();
};

