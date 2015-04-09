#pragma once


#include <vector>

#include "ARenderableObj.h"
#include "Collider.h"
#include "Math/Vector4.h"

namespace talga
{
	class Texture;
	class LuaEngine;

	class PhysSprite : public ARenderableObj
	{
	public:

		static void LUA_REGISTER(LuaEngine* engine);

		PhysSprite(Game* game, vec4 position = vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4 velocity = vec4(0.0f, 0.0f, 0.0f, 0.0f), std::vector<cpAnim> animations = std::vector<cpAnim>(), I32 width = -1, I32 height = -1);
		~PhysSprite();

		PhysSprite(const PhysSprite& cpy);
		PhysSprite& operator=(const PhysSprite& cpy);

		RectCollider& getCollider() { return mCollider; }
		F32 getVx() const { return mVelocity(0); }
		F32 getVy() const { return mVelocity(1); }
		F32 previousX() const { return mPreviousPosition(0); }
		F32 previousY() const { return mPreviousPosition(1); }
		F32 accelerationY() const { return mAcceleration(0); }
		F32 accelerationX() const { return mAcceleration(1); }

		virtual void Update(F32 dt) override;
		virtual void Render(const Camera& camera) override;

		void setVx(F32 value) { mVelocity[0] = value; }
		void setVy(F32 value) { mVelocity[1] = value; }
		void setX(F32 value) { mPosition[0] = value; mCollider.x = value; } //OVERRIDE
		void setY(F32 value) { mPosition[1] = value; mCollider.y = value; } //OVERRIDE
		void setW(F32 value) { mWidth = value; mCollider.w = value; } //OVERRIDE
		void setH(F32 value) { mHeight = value; mCollider.h = value; } //OVERRIDE
		void setAccelerationY(float value) { mAcceleration[1] = value; };
		I32 getListLoc() const { return mListLoc; }
		void setListLoc(I32 value) { mListLoc = value; }
		void setApplyG(bool value) { mApplyG = value; }
		void setVerticalWalled(bool value) { mVerticalWalled = value; }
		void setHorizontalWalled(bool value) { mHorizontalWalled = value; }
		void setIsColliding(bool value) { mIsColliding = value; }
		bool isVerticalWalled() const { return mVerticalWalled; }
		bool isHorizontalWalled() const { return mHorizontalWalled; }
		bool isColliding() const { return mIsColliding; }
		bool checkCollisions() const { return mCheckCollisions; }
		void setCheckCollisions(bool value) { mCheckCollisions = value; }
		bool hasJumped() const { return mHasJumped; }
		void setHasJumped(bool value) { mHasJumped = value; }
	protected:
		I32 mListLoc;
		bool mHasJumped;
		bool mApplyG;
		bool mVerticalWalled;
		bool mHorizontalWalled;
		bool mIsColliding;
		bool mCheckCollisions;

		RectCollider mCollider;
		vec4 mVelocity;
		vec4 mPreviousPosition;
		vec4 mAcceleration;
	};
}