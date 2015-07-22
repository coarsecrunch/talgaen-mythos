#include "LuaEngine.h"

#include "lua/lua.hpp"
#include "oolua/oolua.h"
#include "oolua/oolua_function.h"
#include "oolua/oolua_ref.h"
#include <iostream>
#include <algorithm>
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
			reportError();
	}


	void LuaEngine::reportError()
	{
		std::string err = OOLUA::get_last_error(getState());
		std::cout << "Lua error: " << err << std::endl;;
		mGame->printToLuaPrompt(err);
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

	OOLUA::Lua_func_ref LuaEngine::getGlobalFunction(std::string name)
	{
		OOLUA::Lua_func_ref tempRef(getState());
		if (OOLUA::get_global(getState(), name.c_str(), tempRef))
			;
		else
			reportError();
	
		return tempRef;
	}

	OOLUA::Lua_table_ref LuaEngine::getGlobalTable(std::string name)
	{
		OOLUA::Lua_table_ref tempRef(getState());
		if (OOLUA::get_global(getState(), name.c_str(), tempRef))
			;
		else
			reportError();

		return tempRef;
	}

	void LuaEngine::ExecuteStr(std::string str)
	{
		if (mScript.run_chunk(str))
			;
		else
			reportError();
	}

	LuaEngine::~LuaEngine()
	{
	}
}
