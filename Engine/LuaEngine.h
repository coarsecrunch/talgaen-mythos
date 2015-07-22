#pragma once

//LuaEngine.h must be included BEOFRE luaBridge

#include <string>
#include "oolua/oolua.h"
#include "oolua/oolua_script.h"
#include "oolua/oolua_registration.h"
struct lua_State;


namespace talga
{
	class Game;
	class LuaEngine
	{

	public:
		static LuaEngine* instance();
		
		~LuaEngine();

		lua_State* getState();

		template<typename T>
		void regClass()
		{
			mScript.register_class<T>();
		}
		
		template<typename T>
		void addGlobal(std::string name, T& ref)
		{
			OOLUA::set_global(getState(), name.c_str(), ref);
		}

		OOLUA::Lua_func_ref getGlobalFunction(std::string name);
		OOLUA::Lua_table_ref getGlobalTable(std::string name);

		void stackDump();
		void ExecuteFile(std::string path);
		void ExecuteStr(std::string str);
		void setGame(Game* game) { mGame = game; }
	protected:
		LuaEngine();
		OOLUA::Script mScript;
		Game* mGame;
		void reportError();
	};
}