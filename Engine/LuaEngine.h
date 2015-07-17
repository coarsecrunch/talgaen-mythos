#pragma once

//LuaEngine.h must be included BEOFRE luaBridge

#include <string>

struct lua_State;

namespace talga
{
	class LuaEngine
	{

	public:
		static LuaEngine* instance();

		
		~LuaEngine();

		lua_State* getState();

		void ExecuteFile(std::string path);
		void ExecuteStr(std::string str);
	protected:
		LuaEngine();
		lua_State* mState;

	protected:
		void reportError(int err);
	};
}
