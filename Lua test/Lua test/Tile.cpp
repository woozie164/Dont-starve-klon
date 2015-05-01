#include "Tile.h"
#include "src\SOIL.h"
#include "shaders.h"

GLuint Tile::texture;

extern ShaderProgramManager spm;

Tile::Tile()
{
	vertices.resize(4);

	type = Grass;

	indices.reserve(6);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	
	VRAMDataflag = false;
}


Tile::~Tile()
{
}


Tile::Tile(float width, float depth, float x, float y, float z) : Tile()
{	
	this->width = width;
	this->depth = depth;

	SetPosition(x, y, z);
	SetTileType(type);

	collisionBox.min[0] = x;
	collisionBox.min[1] = z;
	collisionBox.max[0] = x + width;
	collisionBox.max[1] = z + depth;
}


void Tile::LoadTexture(const char * filename)
{
	/*
	Something wrong here. Can't get the texture to display using this way.

	glGenTextures(1, &texture);

	int width, height;
	unsigned char* image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	
	SOIL_free_image_data(image);
	*/
	static bool firstTime = true;
	if (firstTime)
	{
		Tile::texture = SOIL_load_OGL_texture
			(
				"tiles.png",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		if (Tile::texture == 0)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
		firstTime = false;
	}


	/* I can't get this part to work, for some reason
	so i changed the shader to always use texture unit 0
	GLuint shaderProgram = spm.GetShaderProgram("3dplanetex");	
	GLuint l = glGetUniformLocation(shaderProgram, "tex");
	glUniform1i(l, 0);
	//glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
	*/
	InitBuffers();
}

void Tile::InitBuffers()
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * 4, vertices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid *)(3 * sizeof(float)));

	// Element buffers are a part of VAO state
	// so this should come between glBindVertexArray(terrainVAO) and glBindVertexArray(0)	
	glGenBuffers(1, &elementVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Tile::Draw()
{
	if (VRAMDataflag) {
		UpdateVRAMData();
		VRAMDataflag = false;
	}

	glUseProgram(spm.GetShaderProgram("3dplanetex"));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Tile::texture);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tile::SetPosition(float x, float y, float z)
{
	vertices[0].point = glm::vec3(x, y, z);
	vertices[1].point = glm::vec3(x + width, y, z);
	vertices[2].point = glm::vec3(x + width, y, z + depth);
	vertices[3].point = glm::vec3(x, y, z + depth);

	collisionBox.min[0] = x;
	collisionBox.min[1] = z;
	collisionBox.max[0] = x + width;
	collisionBox.max[1] = z + depth;

	VRAMDataflag = true;
}

// Setting the tile type determines what part of the tiles.png image should be rendered
void Tile::SetTileType(TileType type)
{
	this->type = type;
	switch (type)
	{
	case Rock:
		vertices[0].texCoord = glm::vec2(0.0f, 0.0f);
		vertices[1].texCoord = glm::vec2(1.0f / 3.0f, 0.0f);
		vertices[2].texCoord = glm::vec2(1.0f / 3.0f, 1.0f);
		vertices[3].texCoord = glm::vec2(0.0f, 1.0f);
		break;
	case Grass:
		vertices[0].texCoord = glm::vec2(1.0f / 3.0f, 0.0f);
		vertices[1].texCoord = glm::vec2(2.0f / 3.0f, 0.0f);
		vertices[2].texCoord = glm::vec2(2.0f / 3.0f, 1.0f);
		vertices[3].texCoord = glm::vec2(1.0f / 3.0f, 1.0f);
		break;
	case Dirt:
		vertices[0].texCoord = glm::vec2(2.0f / 3.0f, 0.0f);
		vertices[1].texCoord = glm::vec2(1.0f, 0.0f);
		vertices[2].texCoord = glm::vec2(1.0f, 1.0f);
		vertices[3].texCoord = glm::vec2(2.0f / 3.0f, 1.0f);
		break;
	default:
		break;
	}

	VRAMDataflag = true;
}

void Tile::UpdateVRAMData()
{
	// Update the data that's already in the VRAM
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * 4, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}