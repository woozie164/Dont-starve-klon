#include <iostream>
#include <string>
#include "lua.hpp"
#include "glfw3.h"

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

int main( int argc, char ** argv )
{
	cout << "Hello world" << endl;
	GLFWwindow* window;
	
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glBegin(GL_TRIANGLES);
		glVertex2d(0, 0);
		glVertex2d(0, 1);
		glVertex2d(1, 0);
		glEnd();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
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
