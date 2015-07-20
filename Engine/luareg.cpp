#include "luareg.h"
#include "oolua/oolua_dsl_export.h"
#include "LuaEngine.h"

//GameObject
OOLUA_EXPORT_FUNCTIONS(GameObject, addKeyCallback, addCollisionCallback, setCollisionType)
OOLUA_EXPORT_FUNCTIONS_CONST(GameObject)

//Rectangle
OOLUA_EXPORT_FUNCTIONS(Rectangle, setX, setY, setZ)
OOLUA_EXPORT_FUNCTIONS_CONST(Rectangle, getX, getY, getZ)

//Camera
OOLUA_EXPORT_FUNCTIONS(Camera, box)
OOLUA_EXPORT_FUNCTIONS_CONST(Camera)

//Game
OOLUA_EXPORT_FUNCTIONS(Game, camera)
OOLUA_EXPORT_FUNCTIONS_CONST(Game)

void LUA_REGISTER_TYPES()
{
	talga::LuaEngine::instance()->regClass<talga::Rectangle>();
	talga::LuaEngine::instance()->regClass<talga::Camera>();
	talga::LuaEngine::instance()->regClass<talga::GameObject>();
	talga::LuaEngine::instance()->regClass<talga::Game>();
}