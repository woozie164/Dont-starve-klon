#include "lua.hpp"
#include <iostream>
#include <string>
#include "GameObject.h"


extern "C" {
	static int l_GameObject_Create(lua_State * L)
	{
		GameObject** gobj = reinterpret_cast<GameObject**>(lua_newuserdata( L, sizeof( GameObject* ) ));

		*gobj = new GameObject();
		return 1;
	}

	static int l_GameObject_SetPosition(lua_State * L)
	{
		int n = lua_gettop(L);
		if (n != 4)
		{			
			std::cerr << __FUNCTION__ << " expected 4 argument, " << n << " arguments found." << std::endl;
			return 0;
		}
		GameObject** gobj = reinterpret_cast<GameObject**>(lua_touserdata(L,1));
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		(*gobj)->SetPosition(x, y, z);
		//gobj->SetPosition(x, y, z);
		return 0;
	}

	int test(lua_State * L)
	{
		int n = lua_gettop(L);
		if(n != 1)
		{
			std::cerr << "Expected 1 argument, " << n << " arguments found." << std::endl;
			return 0;
		}
		std::cout << lua_tostring(L, 1) << std::endl;
		return 0;
	}
}

// Start the lua interpreter, open standard libs, and expose C++ method to lua
lua_State * L;
void InitLua()
{
	L = luaL_newstate();
	luaL_openlibs(L); //�ppna alla standardlib i lua

	// Expose a C++ metod to Lua
	lua_pushcfunction(L, test);
	lua_setglobal(L, "testCfunc");

	lua_pushcfunction(L, l_GameObject_Create);
	lua_setglobal(L, "GameObject_Create");

	lua_pushcfunction(L, l_GameObject_SetPosition);
	lua_setglobal(L, "GameObject_SetPosition");

	// Run a Lua script file
	int error = luaL_loadfile(L, "lua/functiontest.lua") || lua_pcall(L, 0, 0, 0);
	if( error )
	{
		std::cerr << "Unable to run:" << lua_tostring(L, 1);
		lua_pop(L,1);
	}

	error = luaL_loadfile(L, "lua/LuaAPItest.lua") || lua_pcall(L, 0, 0, 0);
	if( error )
	{
		std::cerr << "Unable to run:" << lua_tostring(L, 1);
		lua_pop(L,1);
	}
}
