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

struct Entity 
{
	float position[3];
	std::string name;
};

int test(lua_State * L)
{
	int n = lua_gettop(L);
	if(n != 1)
	{
		std::cerr << "Expected 1 argument, " << n << " arguments found." << std::endl;
		return 0;
	}
	std::string s = lua_tostring(L, -1);
	std::cout << s << std::endl;
	return 0;
}
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
	cout << "Hello world" << endl;

	
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

	ShaderProgramManager spm;
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
	GLuint terrainVBO;
	GLuint terrainVAO;

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, terrain.GetVertexSize() * terrain.GetVertexCount(), terrain.GetVertexData(), GL_STATIC_DRAW);

	GLuint terrainElementVBO;
	glGenBuffers(1, &terrainElementVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainElementVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrain.GetIndicesCount() * terrain.GetIndicesSize(), terrain.GetIndicesData(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, terrain.GetVertexSize(), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, terrain.GetVertexSize(), (GLvoid *)(3 * sizeof(float)));
	glBindVertexArray(0);
//	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{		
		static double lastTime = glfwGetTime();
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

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

		//glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainElementVBO);
		glBindVertexArray(terrainVAO);
		glDrawElements(GL_TRIANGLES, terrain.GetIndicesCount(), GL_UNSIGNED_INT, 0); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

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
		glClear(GL_COLOR_BUFFER_BIT);
		/* Poll for and process events */
		glfwPollEvents();

		lastTime = currentTime;
	}

	glfwTerminate();
	return 0;

	lua_State * L = luaL_newstate();
	luaL_openlibs(L); //Öppna alla standardlib i lua

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

	// Calling a C++ metod from Lua
	lua_pushcfunction(L, test);
	lua_setglobal(L, "testCfunc");

	error = luaL_loadfile(L, "lua/functiontest.lua") || lua_pcall(L, 0, 0, 0);
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