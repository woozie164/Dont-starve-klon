#include <iostream>
#include <string>
#include "lua.hpp"
#include "GL\glew.h"
#include "glfw3.h"
#include "shaders.h"
#include "glm\glm.hpp"
#include "glm\common.hpp"
#include "glm\ext.hpp"
#include "Camera.h"
#include "terrain.h"
#include "src\SOIL.h"
#include "Tile.h"
#include "GameObject.h"
#include "World.h"


ShaderProgramManager spm;
World world;

 extern void InitLua();
 extern lua_State * L;

struct Entity 
{
	float position[3];
	std::string name;
};

using namespace std;

const GLfloat triangle[] = { 
	-1.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
};

GLuint VBOHandle[1];
GLuint VAOHandle[1];

void InitTriangle()
{
	glGenBuffers(1, VBOHandle);
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
}

void DrawTriangle()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOHandle[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}

void glfw_error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// Ignore certain messages
	switch (id)
	{
	// A verbose message about what type of memory was allocated for a buffer.
	case 131185: 
		return;
	}

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity){
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}

GLFWwindow* window;

int main( int argc, char ** argv )
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	glfwSetErrorCallback(glfw_error_callback);

	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	//window = glfwCreateWindow(1920, 1080, "Borderless fullscreen ftw", NULL, NULL);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&openglCallbackFunction, nullptr);
	}
	else
	{
		cout << "glDebugMessageCallback not available" << endl;
	}
	
	InitTriangle();
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

	spm.LoadShaderFromFile("3dplane.vert", GL_VERTEX_SHADER);
	spm.LoadShaderFromFile("3dplane.frag", GL_FRAGMENT_SHADER);
	GLuint planeProg = spm.CompileShaderProgram("3dplane");

	glUseProgram(planeProg);
	GLuint u_projection = glGetUniformLocation(planeProg, "projection");
	GLuint u_view = glGetUniformLocation(planeProg, "view");

	glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 150.0f);
	glm::vec3 position(0.0f, 0.0f, 0.0f);
	glm::vec3 direction(1.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::mat4 viewMat = glm::lookAt(position, position + direction, up);

	Camera camera;
	Terrain terrain;
	terrain.LoadTerrain(127.0f, 127.0f);

	spm.LoadShaderFromFile("3dplanetex.vert", GL_VERTEX_SHADER);
	spm.LoadShaderFromFile("3dplanetex.frag", GL_FRAGMENT_SHADER);
	GLuint planeTexProg = spm.CompileShaderProgram("3dplanetex");	
	GLuint u_projection2 = glGetUniformLocation(planeTexProg, "projection");
	GLuint u_sampler2D = glGetUniformLocation(planeTexProg, "tex");
	GLuint u_view2 = glGetUniformLocation(planeTexProg, "view");

	GLuint result = SOIL_load_OGL_texture
		(
		"tiles.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	if (result == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	/*
	Tile tile(4.0, 4.0f, 0.0f, 3.0f, 0.0f);
	tile.LoadTexture("tiles.png");

	Tile tile2(4.0f, 4.0f, 4.0f, 3.0f, 0.0f);
	tile2.LoadTexture("tiles.png");
	*/
	// Create a plane that consists of several tiles
	vector<Tile> tiles;
	
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			float width = 4.0f;
			float depth = 4.0f;
			tiles.push_back(Tile(width, depth, i * width, 3.0f, j * depth));
			tiles[j + i * 6].LoadTexture("tiles.png");

			// Give each row a certain tile type
			switch (i % 3)
			{
			case 0:
				tiles[j + i * 6].SetTileType(Grass);
				break;
			case 1:
				tiles[j + i * 6].SetTileType(Dirt);
				break;
			case 2:
				tiles[j + i * 6].SetTileType(Rock);
				break;
			}
			
		}		
	}
	
	GameObject mainCharacter;

	glEnable(GL_DEPTH_TEST);

	InitLua();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static double lastTime = glfwGetTime();
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		glUseProgram(planeProg);

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			/*camera.cameraPosition = glm::vec3(14.1046686f, 5.20885277f, 0.567461133f);
			camera.horizontalAngle = -728.854736f;
			camera.verticalAngle = -120.005180f;
			*/
			camera.type = Camera::ISOMETRIC;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			camera.type = Camera::TOP_DOWN;
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			camera.type = Camera::FREE;
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {			
			mainCharacter.SetPosition(3.0f, 3.0f, 0.0f);
		}

		camera.Update(deltaTime);
		glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(camera.projMat));
		glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(camera.viewMat));

		/* Render here */		
		glBegin(GL_TRIANGLES);
		glVertex2d(0, 0);
		glVertex2d(0, 1);
		glVertex2d(1, 0);
		glEnd();
		
		glBegin(GL_TRIANGLES);
		glVertex3d(1, 0, -0.5);
		glVertex3d(1, 1, 0);
		glVertex3d(1, 0, 0);
		glEnd();
		
		DrawTriangle();
	
		glUseProgram(planeTexProg);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, result);
		glUniformMatrix4fv(u_projection2, 1, GL_FALSE, glm::value_ptr(camera.projMat));
		glUniformMatrix4fv(u_view2, 1, GL_FALSE, glm::value_ptr(camera.viewMat));
		terrain.Draw();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		camera.ScreenToWorldCoord(x, y);
		//tile.Draw();
		//tile2.Draw();
		for ( int i  = 0; i < tiles.size(); i++ )
		{
			tiles[i].Draw();
		}

		mainCharacter.Draw();
		world.Update();
		world.Render();

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			int error = luaL_loadfile(L, "lua/LuaAPItest.lua") || lua_pcall(L, 0, 0, 0);
			if (error)
			{
				std::cerr << "Unable to run:" << lua_tostring(L, 1);
				lua_pop(L, 1);
			}
		}	

	


		glValidateProgram(planeProg);
		GLint result;
		glGetProgramiv(planeProg, GL_VALIDATE_STATUS, &result);
		if (result == GL_FALSE)
		{
			cout << "ERROR WHEN VALIDATING PROGRAM" << endl;
		}

		GLenum e;
		while ((e = glGetError()) != GL_NO_ERROR)
		{
			cout << e;
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		lastTime = currentTime;
	}

	glfwTerminate();
	return 0;

	// Creating a lua table from data in C++
	Entity entity;
	entity.name = "Bob";
	entity.position[0] = 0;
	entity.position[1] = 0;
	entity.position[2] = 0;

	lua_createtable(L, 3, 1);

	int index = lua_gettop(L);
	for(int i = 0; i < 3; i++)
	{
		lua_pushnumber(L, entity.position[i]);
		lua_rawseti(L, index, i + 1);
	}
	lua_pushstring(L, "name");
	lua_pushstring(L, entity.name.c_str());
	lua_settable(L, index);
	lua_setglobal(L, "Entity1");

	int error = luaL_loadfile(L, "lua/printtable.lua") || lua_pcall(L, 0, 0, 0);
	if( error )
	{
		std::cerr << "Unable to run:" << lua_tostring(L, 1);
		lua_pop(L,1);
	}
	

	/*
	int error = luaL_loadstring(L,"print('Hello World!')") || lua_pcall(L,0,0,0);
	if( error )
	{
		std::cerr << "Unable to run:" << lua_tostring(L, 1);
		lua_pop(L,1);
	}
	lua_pushstring(L, "NEJ" );
	lua_setglobal(L, "min_variabel");
	error = luaL_loadfile(L, "J:/Visual Studio 2012/Projects/Lua test/Lua test/lua/script.lua") || lua_pcall(L, 0, 1, 0);
	
	if(error)
	{
		std::cerr << "Unable to run:" << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	else
	{
		std::cout << lua_tostring( L, -1 ) << std::endl;
		lua_pop(L, 1);
	}
	lua_getglobal(L, "min_variabel");
	std::cout << lua_tostring(L, -1);
	std::cout << std::endl;
	lua_pop(L, 1);
	lua_getglobal( L, "errhandler" );
	int errhandler_pos = lua_gettop(L);
	lua_getglobal(L, "sqr");
	lua_pushinteger(L, 100);
	error = lua_pcall(L, 1, 1, errhandler_pos);
	if(error)
	{
		std::cerr << "Unable to run:" << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	if(!error) {
		std::cout << lua_tonumber(L, -1) << std::endl;
		lua_pop(L, 1);
	}
	error = luaL_loadfile(L, "J:/Visual Studio 2012/Projects/Lua test/Lua test/lua/fib.lua") || lua_pcall(L, 0, 0, 0);
		
	if(error)
	{
		std::cerr << "Unable to run:" << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	error = luaL_loadfile(L, "J:/Visual Studio 2012/Projects/Lua test/Lua test/lua/table.lua") || lua_pcall(L, 0, 0, 0);
		
	if(error)
	{
		std::cerr << "Unable to run:" << lua_tostring(L, -1);
		lua_pop(L, 1);
	}
	// table is in the stack at index 't'
	lua_getglobal(L, "test");
    lua_pushnil(L);  // first key 
     while (lua_next(L, -2) != 0) {
       // uses 'key' (at index -2) and 'value' (at index -1) 
       printf("%s - %s\n",
              lua_typename(L, lua_type(L, -2)),
              lua_typename(L, lua_type(L, -1)));
			std:: cout <<lua_tostring(L, -1) << std::endl;
       // removes 'value'; keeps 'key' for next iteration 
       lua_pop(L, 1);
     }
	 */
}