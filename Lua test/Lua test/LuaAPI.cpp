#include "lua.hpp"
#include <iostream>
#include <string>
#include "GameObject.h"
#include "World.h"
#include <Windows.h>

extern World world;


extern "C" {
	static int l_GameObject_Create(lua_State * L)
	{
		GameObject** gobj = reinterpret_cast<GameObject**>(lua_newuserdata( L, sizeof( GameObject* ) ));

		*gobj = new GameObject();
		world.AddGameObject(*gobj);		
		world.AddDrawable((Drawable *)*gobj);
		return 1;
	}

	static int l_GameObject_SetPosition(lua_State * L)
	{
		int n = lua_gettop(L);
		if (n != 4)
		{			
			std::cerr << __FUNCTION__ << " expected 4 arguments, " << n << " arguments found." << std::endl;
			return 0;
		}
		GameObject** gobj = reinterpret_cast<GameObject**>(lua_touserdata(L,1));
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		(*gobj)->SetPosition(x, y, z);
		return 0;
	}

	static int l_GameObject_Delete(lua_State * L)
	{
		int n = lua_gettop(L);
		if (n != 1) {
			std::cerr << __FUNCTION__ << " expected 1 arguments, " << n << " arguments found." << std::endl;
			return 0;
		}

		GameObject** gobj = reinterpret_cast<GameObject**>(lua_touserdata(L, 1));
		world.RemoveGameObject(*gobj);
		world.RemoveDrawable((Drawable *)*gobj);
		delete *gobj;

		return 0;
	}

	static int l_Tile_Create(lua_State * L)
	{
		int n = lua_gettop(L);
		if (n != 5) {
			std::cerr << __FUNCTION__ << " expected 5 arguments, " << n << " arguments found." << std::endl;
			return 0;
		}

		float width = lua_tonumber(L, 1);
		float depth = lua_tonumber(L, 2);
		float x = lua_tonumber(L, 3);
		float y = lua_tonumber(L, 4);
		float z = lua_tonumber(L, 5);

		Tile ** tile = reinterpret_cast<Tile**>(lua_newuserdata(L, sizeof(Tile*)));
		*tile = new Tile(width, depth, x, y, z);
		world.AddDrawable((Drawable *)*tile);
		return 1;
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

void WatchLuaDirectory()
{
	// Register to recieve a notification whenever a Lua script has been changed.
	HANDLE h = FindFirstChangeNotification(
		L"C:/Users/woozie/Documents/GitHub/Dont-starve-klon/Lua test/Lua test/lua",
		true,
		FILE_NOTIFY_CHANGE_LAST_WRITE);
	if (h == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstChangeNotification function failed.\n");
	}
}

// Start the lua interpreter, open standard libs, and expose C++ method to lua
lua_State * L;
void InitLua()
{
	L = luaL_newstate();
	luaL_openlibs(L); //Öppna alla standardlib i lua

	// Expose a C++ metod to Lua
	lua_pushcfunction(L, test);
	lua_setglobal(L, "testCfunc");

	lua_pushcfunction(L, l_GameObject_Create);
	lua_setglobal(L, "GameObject_Create");

	lua_pushcfunction(L, l_GameObject_SetPosition);
	lua_setglobal(L, "GameObject_SetPosition");

	lua_pushcfunction(L, l_GameObject_Delete);
	lua_setglobal(L, "GameObject_Delete");

	lua_pushcfunction(L, l_Tile_Create);
	lua_setglobal(L, "Tile_Create");

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

	WatchLuaDirectory();
}
