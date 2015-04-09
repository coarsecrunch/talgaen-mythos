#include "PhysSprite.h"
#include "TMath.h"
#include "Animation.h"
#include "Texture.h"
#include "Collider.h"
#include "Camera.h"
#include "Game.h"
#include "LuaEngine.h"
#include "LuaBridge\LuaBridge.h"


namespace talga
{
	void PhysSprite::LUA_REGISTER(LuaEngine* engine)
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
			.endNamespace();
	}

	PhysSprite::PhysSprite(Game* game, vec4 position, vec4 velocity, std::vector<cpAnim> animations, I32 width, I32 height)
		: ARenderableObj(game, animations, position, width, height)
		, mVelocity(velocity)
		, mPreviousPosition(position)
		, mAcceleration(vec4(0.0f, 0.0f, 0.0f, 0.0f))
		, mCollider(0, 0, 0, 0)
		, mListLoc(-1)
		, mApplyG(true)
		, mVerticalWalled(false)
		, mHorizontalWalled(false)
		, mIsColliding(false)
		, mCheckCollisions(true)
	{
	}

	PhysSprite::PhysSprite(const PhysSprite& cpy)
		: ARenderableObj(cpy)
		, mVelocity(cpy.mVelocity)
		, mPreviousPosition(cpy.mPreviousPosition)
		, mAcceleration(cpy.mAcceleration)
		, mCollider(cpy.mCollider)
		, mIsColliding(cpy.mIsColliding)
		, mApplyG(cpy.mApplyG)
		, mVerticalWalled(cpy.mVerticalWalled)
		, mHorizontalWalled(cpy.mHorizontalWalled)
	{
		if (mAnimations.size() > 0)
			mCurrentAnimation = mAnimations[0];

		if (mCurrentAnimation)
		{
			mCollider = RectCollider(getX(), getY(), mCurrentAnimation->frames[mCurrentFrame].w, mCurrentAnimation->frames[mCurrentFrame].h);
		}
		else
		{
			setW(0.0f);
			setH(0.0f);
			setVisible(false);
			setApplyG(false);
			mCollider = RectCollider(getX(), getY(), mWidth, mHeight);
		}

	}

	PhysSprite& PhysSprite::operator=(const PhysSprite& cpy)
	{
		ARenderableObj::operator=(cpy);

		mListLoc = cpy.mListLoc;
		mHasJumped = cpy.mHasJumped;
		mApplyG = cpy.mApplyG;
		mVerticalWalled = cpy.mVerticalWalled;
		mHorizontalWalled = cpy.mHorizontalWalled;
		mIsColliding = cpy.mIsColliding;

		mCollider = cpy.mCollider;
		mVelocity = cpy.mVelocity;
		mPreviousPosition = cpy.mPreviousPosition;
		mAcceleration = cpy.mAcceleration;

		return *this;
	}

	//collider MUST be updated with the actual sprite
	void PhysSprite::Update(F32 dt)
	{
		ARenderableObj::Update(dt);

		if (mApplyG)
			mAcceleration[1] = GRAVITY* dt * DT_VEL_RATIO;

		mVelocity = mVelocity + mAcceleration;

		if (getVx() > MAX_SPEED)
			setVx(MAX_SPEED);
		if (getVy() > MAX_SPEED)
			setVy(MAX_SPEED);

		mPreviousPosition = mPosition;

		mPosition = mPosition + mVelocity * dt * DT_VEL_RATIO;

		if (mCurrentAnimation)
			mCollider = RectCollider(getX(), getY(), mCurrentAnimation->frames[mCurrentFrame].w, mCurrentAnimation->frames[mCurrentFrame].h);
	}

	void PhysSprite::Render(const Camera& camera)
	{
		glBindVertexArray(Camera::mVAOs[Camera::TEXTURED_QUAD]);
		glUseProgram(Camera::mProgram);
		glViewport(0, 0, camera.getW(), camera.getH());
		glBindVertexArray(Camera::mVAOs[Camera::TEXTURED_QUAD]);

		getAnimation()->tex->Bind();

		mat4 fix = getMTWMat();

		fix = camera.getCameraMat();

		fix = camera.getProjectionMatrix() * camera.getCameraMat() * getMTWMat();

		glUniformMatrix4fv(Camera::MVPShaderLoc, 1, GL_TRUE, &fix[0][0]);

		unsigned int offset = getAnimation()->getOffset(getFrame());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)offset);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glFlush();
	}

	PhysSprite::~PhysSprite()
	{
	}
}