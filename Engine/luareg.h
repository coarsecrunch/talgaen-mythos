#pragma once

#include "oolua/oolua_dsl.h"
#include "oolua/proxy_tags.h"

#include <string>

#include "Game.h"
#include "Camera.h"
#include "Rectangle.h"
#include "GameObject.h"
namespace OOLUA
{
	typedef talga::Game Game;
	typedef talga::Camera Camera;
	typedef talga::Rectangle Rectangle;
	typedef talga::GameObject GameObject;
}

OOLUA_PROXY(Rectangle)
	OOLUA_TAGS(OOLUA::No_public_constructors)
	OOLUA_MFUNC_CONST(getX)
	OOLUA_MFUNC_CONST(getY)
	OOLUA_MFUNC_CONST(getZ)
	OOLUA_MFUNC(setX)
	OOLUA_MFUNC(setY)
	OOLUA_MFUNC(setZ)
OOLUA_PROXY_END

OOLUA_PROXY(GameObject)
	OOLUA_TAGS(OOLUA::No_public_constructors)
	OOLUA_MEM_FUNC(void, addKeyCallback, std::string, Lua_func_ref)
	OOLUA_MEM_FUNC(void, addCollisionCallback, int, Lua_func_ref)
	OOLUA_MEM_FUNC(void, setCollisionType, int)
OOLUA_PROXY_END

OOLUA_PROXY(Camera)
	OOLUA_TAGS(OOLUA::No_public_constructors)
	OOLUA_MEM_FUNC(Rectangle&, box)
OOLUA_PROXY_END


OOLUA_PROXY(Game)
OOLUA_TAGS(OOLUA::No_public_constructors)
OOLUA_MEM_FUNC(Camera&, camera)
OOLUA_MEM_FUNC_RENAME(printToLuaPromptStr, void, printToLuaPrompt, const std::string&)
OOLUA_MEM_FUNC_RENAME(printToLuaPromptFl, void, printToLuaPrompt, float)
OOLUA_PROXY_END

void LUA_REGISTER_TYPES();