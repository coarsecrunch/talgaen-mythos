#pragma once

#include "PhysSprite.h"
#include "Cmn.h"

namespace talga
{
	class LuaEngine;

	class Character : public PhysSprite
	{
	public:
		static void LUA_REGISTER(LuaEngine* engine);

		Character(Game* game, vec4 position = vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4 velocity = vec4(0.0f, 0.0f, 0.0f, 0.0f), std::vector<cpAnim> animations = std::vector<cpAnim>());
		~Character();

		enum DIRECTION { RIGHT, LEFT };
		enum class ANIM { WALKR = 0, WALKL = 1, STANDR = 2, STANDL = 3, RUNR = 4, RUNL = 5, numAnimations = 6 };

		void jump();
		void walkR();
		void walkL();
		void standR();
		void standL();
		void runR();
		void runL();

		static const F32 JUMP_SPEED;

		DIRECTION getDirection() const { return mDirection; }
		void setDirection(DIRECTION value) { mDirection = value; }

	protected:
		F32 mWalkSpeed;
		F32 mRunSpeed;
		DIRECTION mDirection;
	};
}
