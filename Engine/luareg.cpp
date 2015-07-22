#include "luareg.h"
#include "oolua/oolua_dsl_export.h"
#include "LuaEngine.h"

//Texture
OOLUA_EXPORT_FUNCTIONS(Texture)
OOLUA_EXPORT_FUNCTIONS_CONST(Texture)

//IRenderable
OOLUA_EXPORT_FUNCTIONS(IRenderable)
OOLUA_EXPORT_FUNCTIONS_CONST(IRenderable)

//PhysComp
OOLUA_EXPORT_FUNCTIONS(PhysicsComponent)
OOLUA_EXPORT_FUNCTIONS_CONST(PhysicsComponent)

//RectCollider
OOLUA_EXPORT_FUNCTIONS(RectCollider)
OOLUA_EXPORT_FUNCTIONS_CONST(RectCollider)

//AssetManager
OOLUA_EXPORT_FUNCTIONS(AssetManager, AddTexture)
OOLUA_EXPORT_FUNCTIONS_CONST(AssetManager, GetTexture)

//Sprite
OOLUA_EXPORT_FUNCTIONS(Sprite)
OOLUA_EXPORT_FUNCTIONS_CONST(Sprite)

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
OOLUA_EXPORT_FUNCTIONS(Game, camera, manager, addObj, printToLuaPromptStr, printToLuaPromptFl)
OOLUA_EXPORT_FUNCTIONS_CONST(Game)

void LUA_REGISTER_TYPES()
{
	talga::LuaEngine::instance()->regClass<talga::Texture>();
	talga::LuaEngine::instance()->regClass<talga::IRenderable>();
	talga::LuaEngine::instance()->regClass<talga::PhysicsComponent>();
	talga::LuaEngine::instance()->regClass<talga::RectCollider>();
	talga::LuaEngine::instance()->regClass<talga::AssetManager>();
	talga::LuaEngine::instance()->regClass<talga::Sprite>();
	talga::LuaEngine::instance()->regClass<talga::Rectangle>();
	talga::LuaEngine::instance()->regClass<talga::Camera>();
	talga::LuaEngine::instance()->regClass<talga::GameObject>();
	talga::LuaEngine::instance()->regClass<talga::Game>();
}