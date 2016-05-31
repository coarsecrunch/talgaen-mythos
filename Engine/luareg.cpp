#include "luareg.h"
#include "oolua/oolua_dsl_export.h"
#include "LuaEngine.h"

//Texture
OOLUA_EXPORT_FUNCTIONS(vec2)
OOLUA_EXPORT_FUNCTIONS_CONST(vec2, x, y)

OOLUA_EXPORT_FUNCTIONS(vec3)
OOLUA_EXPORT_FUNCTIONS_CONST(vec3, x, y, z)

OOLUA_EXPORT_FUNCTIONS(Texture)
OOLUA_EXPORT_FUNCTIONS_CONST(Texture)

//IRenderable
OOLUA_EXPORT_FUNCTIONS(IRenderable)
OOLUA_EXPORT_FUNCTIONS_CONST(IRenderable)
//AnimSet
OOLUA_EXPORT_FUNCTIONS(AnimationSet)
OOLUA_EXPORT_FUNCTIONS_CONST(AnimationSet)

//PhysComp
OOLUA_EXPORT_FUNCTIONS(PhysicsComponent, setFriction, setMoment, setMass, applyForceX, applyForceY,
	applyImpulseX, applyImpulseY, setX, setY)
OOLUA_EXPORT_FUNCTIONS_CONST(PhysicsComponent, getFriction, getMoment, getMass, getX, getY,
		getVx, getVy, getOrientation)

OOLUA_EXPORT_FUNCTIONS(CollisionData)
OOLUA_EXPORT_FUNCTIONS_CONST(CollisionData, norm)

//RectCollider
OOLUA_EXPORT_FUNCTIONS(RectCollider)
OOLUA_EXPORT_FUNCTIONS_CONST(RectCollider)

//AssetManager
OOLUA_EXPORT_FUNCTIONS(AssetManager, AddTexture, AddAnimationSet)

OOLUA_EXPORT_FUNCTIONS_CONST(AssetManager, GetTexture)

//Sprite
OOLUA_EXPORT_FUNCTIONS(Sprite)

OOLUA_EXPORT_FUNCTIONS_CONST(Sprite)

//Sprite
OOLUA_EXPORT_FUNCTIONS(AnimSprite)
OOLUA_EXPORT_FUNCTIONS_CONST(AnimSprite)

//GameObject
OOLUA_EXPORT_FUNCTIONS(GameObject, addKeyCallback, addCollisionCallback,
addDefaultCollisionCallback, setCollisionType, setCollider, setRenderable, playAnimation, getCollider)
OOLUA_EXPORT_FUNCTIONS_CONST(GameObject)

//Rectangle
OOLUA_EXPORT_FUNCTIONS(Rectangle, setX, setY, setZ)
OOLUA_EXPORT_FUNCTIONS_CONST(Rectangle, getX, getY, getZ)

//Camera
OOLUA_EXPORT_FUNCTIONS(Camera, box)
OOLUA_EXPORT_FUNCTIONS_CONST(Camera)

//Game
OOLUA_EXPORT_FUNCTIONS(Game, camera, manager, addObj, removeObj, setGravity, printToLuaPromptStr, printToLuaPromptFl)
OOLUA_EXPORT_FUNCTIONS_CONST(Game)

void LUA_REGISTER_TYPES()
{
	talga::LuaEngine::instance()->regClass<talga::vec2>();
	talga::LuaEngine::instance()->regClass<talga::vec3>();
	talga::LuaEngine::instance()->regClass<talga::Texture>();
	talga::LuaEngine::instance()->regClass<talga::IRenderable>();
	talga::LuaEngine::instance()->regClass<talga::AnimationSet>();
	talga::LuaEngine::instance()->regClass<talga::PhysicsComponent>();
	talga::LuaEngine::instance()->regClass<talga::LuaCollisionData>();
	talga::LuaEngine::instance()->regClass<talga::RectCollider>();
	talga::LuaEngine::instance()->regClass<talga::AssetManager>();
	talga::LuaEngine::instance()->regClass<talga::Sprite>();
	talga::LuaEngine::instance()->regClass<talga::AnimSprite>();
	talga::LuaEngine::instance()->regClass<talga::Rectangle>();
	talga::LuaEngine::instance()->regClass<talga::Camera>();
	talga::LuaEngine::instance()->regClass<talga::GameObject>();
	talga::LuaEngine::instance()->regClass<talga::Game>();
}