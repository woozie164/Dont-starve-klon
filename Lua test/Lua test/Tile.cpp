#include "Tile.h"
#include "src\SOIL.h"
#include "shaders.h"

extern ShaderProgramManager spm;

Tile::Tile()
{

}


Tile::~Tile()
{
}


Tile::Tile(float width, float depth, float x, float y, float z)
{
	VertexData vert;
	vert.point = glm::vec3(x, y, z);
	vert.texCoord = glm::vec2(0.0f, 0.0f);
	vertices.push_back(vert);
	
	vert.point = glm::vec3(x + width, y, z);
	vert.texCoord = glm::vec2(1.0f, 0.0f);
	vertices.push_back(vert);

	vert.point = glm::vec3(x + width, y, z + depth);
	vert.texCoord = glm::vec2(1.0f, 1.0f);
	vertices.push_back(vert);

	vert.point = glm::vec3(x, y, z + depth);
	vert.texCoord = glm::vec2(0.0f, 1.0f);
	vertices.push_back(vert);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
}


void Tile::LoadTexture(const char * filename)
{
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
	glUseProgram(spm.GetShaderProgram("3dplanetex"));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}