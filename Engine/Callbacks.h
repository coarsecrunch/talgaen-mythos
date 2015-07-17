#pragma once

#include <functional>

#include "LuaEngine.h"
#include "LuaBridge/LuaBridge.h"

namespace talga
{
	class GameObject;

	class StagedFunc
	{
	public:
		StagedFunc() : luaFunc(LuaEngine::instance()->getState()) {}

		static void LUA_REGISTER(LuaEngine* engine)
		{
			using namespace luabridge;

			getGlobalNamespace(engine->getState())
				.beginNamespace("talga")
				.beginClass<StagedFunc>("StagedFunc")
				.endClass()
				.endNamespace();
		}

		void operator=(luabridge::LuaRef ref)
		{
			isLua = true;
			luaFunc = ref;
		}
		void operator=(std::function<void(GameObject*)> func)
		{
			isLua = true;
			stdFunc = func;
		}
		
		void operator()(GameObject* self)
		{
			if (isLua)
				luaFunc(self);
			else
				stdFunc(self);
		}

		operator bool() const
		{
			if (isLua)
			{
				if (luaFunc)
					return true;
				else
					return false;
			}
			else
			{
				if (stdFunc)
					return true;
				else
					return false;
			}
				

		}

	private:
		bool isLua;
		luabridge::LuaRef luaFunc;
		std::function<void(GameObject*)> stdFunc;
	};

	class UpdateFunc
	{
	public:
		UpdateFunc() : luaFunc(LuaEngine::instance()->getState()) {}
		
		static void LUA_REGISTER(LuaEngine* engine)
		{
			using namespace luabridge;

			getGlobalNamespace(engine->getState())
				.beginNamespace("talga")
				.beginClass<UpdateFunc>("UpdateFunc")
				.endClass()
				.endNamespace();
		}

		void operator=(luabridge::LuaRef ref)
		{
			isLua = true;
			luaFunc = ref;
			
		}
		void operator=(std::function<void(GameObject*, I32)> func)
		{
			isLua = true;
			stdFunc = func;
		}

		void operator()(GameObject* self, I32 ms)
		{
			if (isLua)
				luaFunc(self, ms);
			else
				stdFunc(self, ms);
		}

		operator bool() const
		{
			if (isLua)
			{
				if (luaFunc)
					return true;
				else
					return false;
			}
			else
			{
				if (stdFunc)
					return true;
				else
					return false;
			}
		}

	private:
		bool isLua;
		luabridge::LuaRef luaFunc;
		std::function<void(GameObject*, I32)> stdFunc;
	};

	class UnstagedFunc
	{
	public:
		UnstagedFunc() : luaFunc(LuaEngine::instance()->getState()) {}


		void operator=(luabridge::LuaRef ref)
		{
			isLua = true;
			luaFunc = ref;
		}
		void operator=(std::function<void(GameObject*)> func)
		{
			isLua = true;
			stdFunc = func;
		}

		void operator()(GameObject* self)
		{
			if (isLua)
				luaFunc(self);
			else
				stdFunc(self);
		}

		operator bool() const
		{
			if (isLua)
			{
				if (luaFunc)
					return true;
				else
					return false;
			}
			else
			{
				if (stdFunc)
					return true;
				else
					return false;
			}


		}

	private:
		bool isLua;
		luabridge::LuaRef luaFunc;
		std::function<void(GameObject*)> stdFunc;
	};

	
}