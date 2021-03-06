#pragma once
#include <vector>
#include "GameObject.h"

class World
{
	std::vector<GameObject *> gameObjects;
	std::vector<Drawable *> drawables;
	std::vector<Serializable *> serializables;
public:
	World();
	~World();

	void Update();
	void Render();

	void AddGameObject(GameObject * gobj);
	void AddDrawable(Drawable * drawable);
	void AddSerializable(Serializable * serializable);

	void RemoveGameObject(GameObject * gobj);
	void RemoveDrawable(Drawable * drawable);

	void LoadWorld(const char * filename);
	void SaveWorld(const char * filename);
};

