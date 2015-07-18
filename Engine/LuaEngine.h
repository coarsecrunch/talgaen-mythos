#pragma once

//LuaEngine.h must be included BEOFRE luaBridge

#include <string>
#include "oolua/oolua_script.h"
#include "oolua/oolua_registration.h"
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
			if (mScript.push(ref))
				;
			else
				reportError(0);
		}

		template<typename T>
		void addGlobal(std::string name, T& ref)
		{
			OOLUA::set_global(getState(), name.c_str(), ref);
		}

		void stackDump();
		void ExecuteFile(std::string path);
		void ExecuteStr(std::string str);

	protected:
		LuaEngine();
		OOLUA::Script mScript;
	protected:
		void reportError(int err);
	};
}