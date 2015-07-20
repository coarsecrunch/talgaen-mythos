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
				luaFunc(ref, obj, ms);
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
				luaFunc(ref, obj);
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
				luaFunc(ref, obj, action);
			}
			else
			{
				stdFunc(obj, action);
			}
		}
	}

	void CollisionCallbackFunc::operator()(GameObject* obj)
	{
		if (*this)
		{
			if (isLua == TALGA_FUNC_TYPE::LUA)
			{
				luaFunc(ref, obj);
			}
			else
			{
				stdFunc(obj);
			}
		}
	}
}