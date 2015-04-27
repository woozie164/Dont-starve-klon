#include "World.h"
#include "Drawable.h"
#include <iostream>

World::World()
{
}


World::~World()
{
}

void World::AddGameObject(GameObject * gobj)
{
	gameObjects.push_back(gobj);
}

void World::AddDrawable(Drawable * drawable)
{
	drawables.push_back(drawable);
}


void World::RemoveGameObject(GameObject * gobj)
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (*it == gobj)
		{
			gameObjects.erase(it);
			break;
		}
	}
}


void World::RemoveDrawable(Drawable * drawable)
{
	for (auto it = drawables.begin(); it != drawables.end(); ++it)
	{
		if (*it == drawable)
		{
			drawables.erase(it);
			break;
		}
	}
}

void World::Update()
{
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update();
	}

	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		for (unsigned int j = 0; j < gameObjects.size(); j++)
		{
			// Don't collide with yourself
			if (i == j)
				continue;
			if (gameObjects[i]->collisionBox.Intersect(gameObjects[j]->collisionBox))
			{			
				std::cout << "Collision!" << std::endl;
			}
		}
	}
}

void World::Render()
{
	for (unsigned int i = 0; i < drawables.size(); i++)
	{
		drawables[i]->Draw();
	}
}
