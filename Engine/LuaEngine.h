#pragma once

//LuaEngine.h must be included BEOFRE luaBridge

#include <string>
#include "oolua/oolua_script.h"
struct lua_State;

namespace talga
{
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
		void push(const T& ref)
		{
			mScript.push(ref);
		}

		void ExecuteFile(std::string path);
		void ExecuteStr(std::string str);

	protected:
		LuaEngine();
		OOLUA::Script mScript;
	protected:
		void reportError(int err);
	};
}
