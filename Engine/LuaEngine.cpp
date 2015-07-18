#include "LuaEngine.h"

#include "lua/lua.hpp"
#include <iostream>
#include "Game.h"


namespace talga
{
	LuaEngine::LuaEngine() 
		: mScript()
	{
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
		if (mScript.run_file(path))
			;
		else
			reportError(0);
	}


	void LuaEngine::reportError(int err)
	{
		std::cout << "Lua error: " << OOLUA::get_last_error(getState()) << std::endl;;
	}

	void LuaEngine::stackDump()
	{
		lua_Debug entry;
		int depth = 0;

		while (lua_getstack(getState(), depth, &entry))
		{
			int status = lua_getinfo(getState(), "Sln", &entry);
			assert(status);

			std::cout << entry.short_src << "(" << entry.currentline << "): " << (entry.name ? entry.name : "?") << std::endl;
			depth++;
		}
	}

	void LuaEngine::ExecuteStr(std::string str)
	{
		if (mScript.run_chunk(str))
			;
		else
			reportError(0);
	}

	LuaEngine::~LuaEngine()
	{
	}
}
