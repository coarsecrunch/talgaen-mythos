#pragma once

//LuaEngine.h must be included BEOFRE luaBridge

#include <string>

struct lua_State;

namespace talga
{
	class LuaEngine
	{

	public:
		LuaEngine();
		~LuaEngine();

		lua_State* getState();

		void Init();
		void ExecuteFile(std::string path);
		void ExecuteStr(std::string str);
	protected:
		lua_State* mState;

	protected:
		void reportError(int err);
	};
}
