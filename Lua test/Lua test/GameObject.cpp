#include "GameObject.h"
#include <iostream>
#include "shaders.h"

GameObject::GameObject()
{
	int x = 0.0f;
	int y = 3.0f;
	int z = 0.0f;
	int w = 1.0f;
	int d = 1.0f;
	billbordTile = new Tile(w, d, x, y, z);
	billbordTile->LoadTexture("tiles.png");
	billbordTile->SetTileType(Grass);
	collisionBox.max[0] = 1.0f;
	collisionBox.max[1] = 1.0f;
	collisionBox.min[0] = 0;
	collisionBox.min[1] = 0;
	SetPosition(x, y, z);
	/*
	billbordTile->vertices[0].point = glm::vec3(x, y, z);
	billbordTile->vertices[1].point = glm::vec3(x + w, y, z);
	billbordTile->vertices[2].point = glm::vec3(x + w, y + d, z);
	billbordTile->vertices[3].point = glm::vec3(x, y + d, z);

	*/
}


GameObject::~GameObject()
{
}
extern ShaderProgramManager spm;
void GameObject::Draw()
{
	billbordTile->Draw();
	glUseProgram(spm.GetShaderProgram("3dplanetex"));
	glPointSize(100.0f);
	glBegin(GL_QUADS);
	glVertex3f(collisionBox.max[0], 3.0f, collisionBox.max[1]);
	glVertex3f(collisionBox.min[0], 3.0f, collisionBox.min[1]);
	glVertex3f(collisionBox.min[0] + 1.0f, 3.0f, collisionBox.min[1]);
	glVertex3f(collisionBox.min[0], 3.0f, collisionBox.min[1] + 1.0f);
	glEnd();
	glUseProgram(0);
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
	collisionBox.SetPosition(x + w / 2.0f, z);
}

void GameObject::SetPosition(glm::vec3 position)
{
	SetPosition(position.x, position.y, position.z);
}

void GameObject::Serialize(std::ostream & s)
{
	s << "GameObject_Create()" << std::endl;
}