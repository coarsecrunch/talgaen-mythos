#include "LuaEngine.h"

#include "lua/lua.hpp"
#include <iostream>
#include "LuaBridge\LuaBridge.h"
#include "Game.h"

#include "luareg.h"

void printJelly()
{
	std::cout << "I've got jelly in my belly!" << std::endl;
}

OOLUA_CFUNC(printJelly, l_printJelly)
namespace talga
{
	LuaEngine::LuaEngine() 
		: mScript()
	{
		OOLUA::set_global(mScript, "printJelly", l_printJelly);
	}

	LuaEngine* LuaEngine::instance()
	{
		static LuaEngine* engine = new LuaEngine();

		return engine;
	}

	lua_State* LuaEngine::getState()
	{
		return mScript.state();
	}

	void LuaEngine::ExecuteFile(std::string path)
	{
		int err = mScript.run_file(path);
//		reportError(err);
	}

	void LuaEngine::reportError(int err)
	{
		if (err != 0)
		{
			std::cout << "Lua error: " << lua_tostring(getState(), err);
			lua_pop(getState(), 1);
		}
	}

	void LuaEngine::ExecuteStr(std::string str)
	{
		int err = mScript.run_chunk(str);
		reportError(err);
	}

	LuaEngine::~LuaEngine()
	{
	}
}
