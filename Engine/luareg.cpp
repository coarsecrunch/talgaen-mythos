#include "luareg.h"
#include "oolua/oolua_dsl_export.h"
#include "LuaEngine.h"

//OOLUA_EXPORT_FUNCTIONS_CONST(Game, printJelly)

void LUA_REGISTER_TYPES()
{
	talga::LuaEngine::instance()->regClass<talga::Rectangle>();
	talga::LuaEngine::instance()->regClass<talga::Camera>();
	talga::LuaEngine::instance()->regClass<talga::Game>();
}

//Rectangle
OOLUA_EXPORT_FUNCTIONS(Rectangle, setX, setY, setZ)
OOLUA_EXPORT_FUNCTIONS_CONST(Rectangle, getX, getY, getZ)

//Camera
OOLUA_EXPORT_FUNCTIONS(Camera, box)
OOLUA_EXPORT_FUNCTIONS_CONST(Camera)


//Game
OOLUA_EXPORT_FUNCTIONS(Game, camera)
OOLUA_EXPORT_FUNCTIONS_CONST(Game)

