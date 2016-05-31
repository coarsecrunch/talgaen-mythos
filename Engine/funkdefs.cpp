#include "funkdefs.h"
#include "luareg.h"
namespace talga
{
	void UpdateFunc::operator()(GameObject* obj, I32 ms)
	{
		if (*this)
		{
			if (isLua == TALGA_FUNC_TYPE::LUA)
			{
				if (!luaFunc(ref, obj, ms))
					LuaEngine::instance()->reportError();
			}
			else
			{
				stdFunc(obj, ms);
			}
		}
	}

	void StagedFunc::operator()(GameObject* obj)
	{
		if (*this)
		{
			if (isLua == TALGA_FUNC_TYPE::LUA)
			{
				if (!luaFunc(ref, obj))
					LuaEngine::instance()->reportError();
			}
			else
			{
				stdFunc(obj);
			}
		}
	}

	void KeyCallbackFunc::operator()(GameObject* obj, I32 action)
	{
		if (*this)
		{
			if (isLua == TALGA_FUNC_TYPE::LUA)
			{
				if (!luaFunc(ref, obj, action))
					LuaEngine::instance()->reportError();
			}
			else
			{
				stdFunc(obj, action);
			}
		}
	}

	void CollisionCallbackFunc::operator()(GameObject* obj, const LuaCollisionData* data)
	{
		if (*this)
		{
			if (isLua == TALGA_FUNC_TYPE::LUA)
			{
				if (!luaFunc(ref, obj, data))
					LuaEngine::instance()->reportError();
			}
			else
			{
				stdFunc(obj, data);
			}
		}
	}
}