#include "Character.h"
#include <cassert>
#include <iostream>

#include "LuaEngine.h"
#include "LuaBridge/LuaBridge.h"

namespace talga
{
	const F32 Character::JUMP_SPEED = -7.0f;

	void Character::LUA_REGISTER(LuaEngine* engine)
	{
		using namespace luabridge;
		getGlobalNamespace(engine->getState())
			.beginNamespace("Engine")
			.beginClass<PhysSprite>("Actor")
			.addConstructor<void(*)(Game* game, vec4 position, vec4 velocity, std::vector<cpAnim> animations)>()
			.addProperty("vx", &PhysSprite::getVx, &PhysSprite::setVx)
			.addProperty("x", &PhysSprite::getX, &PhysSprite::setX)
			.addProperty("vy", &PhysSprite::getVy, &PhysSprite::setVy)
			.addProperty("y", &PhysSprite::getY, &PhysSprite::setY)
			.endClass()
			.deriveClass<Character, PhysSprite>("Character")
			.addConstructor<void(*)(Game* game, vec4 position, vec4 velocity, std::vector<cpAnim> animations)>()
			.addFunction("jump", &Character::jump)
			.addFunction("walkR", &Character::walkR)
			.addFunction("walkL", &Character::walkL)
			.addFunction("runR", &Character::runR)
			.addFunction("runL", &Character::runL)
			.addFunction("standR", &Character::standR)
			.addFunction("standL", &Character::standL)
			.endClass()
			.endNamespace();
	}

	Character::Character(Game* game, vec4 position, vec4 velocity, std::vector<cpAnim> animations)
		: PhysSprite(game, position, velocity, animations)
		, mRunSpeed(5.0f)
		, mWalkSpeed(2.0f)
	{
		PlayAnimation((I32)ANIM::STANDR, 2000, true);
	}

	void Character::jump()
	{
		if (mHasJumped)
			return;

		setVy(JUMP_SPEED);
		setVerticalWalled(false);
		setHasJumped(true);
	}

	void Character::walkR()
	{
		PlayAnimation((I32)ANIM::WALKR, 1000, true);
		setVx(mWalkSpeed);
		setDirection(DIRECTION::RIGHT);
	}

	void Character::walkL()
	{
		PlayAnimation((I32)ANIM::WALKL, 1000, true);
		setDirection(DIRECTION::LEFT);
		setVx(-mWalkSpeed);
	}

	void Character::standR()
	{
		PlayAnimation((I32)ANIM::STANDR, 2000, true);
		setVx(0.0f);
		setDirection(DIRECTION::RIGHT);
	}

	void Character::standL()
	{
		PlayAnimation((I32)ANIM::STANDL, 2000, true);
		setDirection(DIRECTION::LEFT);
		setVx(0.0f);
	}

	void Character::runR()
	{
		PlayAnimation((I32)ANIM::RUNR, 500, true);
		setDirection(DIRECTION::RIGHT);
		setVx(mRunSpeed);
	}

	void Character::runL()
	{
		PlayAnimation((I32)ANIM::RUNL, 500, true);
		setDirection(DIRECTION::LEFT);
		setVx(-mRunSpeed);
	}

	Character::~Character()
	{
	}
}