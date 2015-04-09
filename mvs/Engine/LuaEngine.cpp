#include "LuaEngine.h"

#include "lua\lua.hpp"
#include <iostream>

namespace talga
{
	LuaEngine::LuaEngine() :
		mState(nullptr)
	{

	}

	lua_State* LuaEngine::getState()
	{
		return mState;
	}

	void LuaEngine::Init()
	{
		mState = luaL_newstate();
		luaL_openlibs(mState);
	}

	void LuaEngine::ExecuteFile(std::string path)
	{
		int err = luaL_dofile(mState, path.c_str());
		reportError(err);
	}

	void LuaEngine::reportError(int err)
	{
		if (err != 0)
		{
			std::cout << "Lua error: " << lua_tostring(mState, err);
			lua_pop(mState, 1);
		}
	}

	void LuaEngine::ExecuteStr(std::string str)
	{
		int err = luaL_dostring(mState, str.c_str());
		reportError(err);
	}

	LuaEngine::~LuaEngine()
	{
	}
}