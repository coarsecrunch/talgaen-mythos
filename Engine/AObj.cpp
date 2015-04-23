#include "Math/Matrix3x3.h"
#include "AObj.h"
#include "Rendering.h"
#include "Game.h"
#include <cmath>

namespace talga
{

	AObj::AObj(Game* game, I32 width, I32 height, vec4 position)
		: mWidth(width)
		, mHeight(height)
		, mPosition(position)
		, mScaleX(1.0f)
		, mScaleY(1.0f)
		, mRotation(0.0f)
		, mGame(game)
	{
	}

	AObj::AObj(const AObj& cpy)
		: mWidth(cpy.mWidth)
		, mHeight(cpy.mHeight)
		, mPosition(cpy.mPosition)
		, mScaleX(1.0f)
		, mScaleY(1.0f)
		, mRotation(0.0f)
		, mGame(cpy.mGame)
	{
	}

	const AObj& AObj::operator=(const AObj& cpy)
	{
		mPosition = cpy.mPosition;
		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mScaleX = cpy.mScaleX;
		mScaleY = cpy.mScaleY;
		mRotation = cpy.mRotation;
		mGame = cpy.mGame;

		return *this;
	}

	mat4 AObj::getRotMat() const
	{
		return mat4(cos(mRotation), -sin(mRotation), 0.0f, 0.0f,
			sin(mRotation), cos(mRotation), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4 AObj::getScaleMat() const
	{
		return mat4(mScaleX, 0.0f, 0.0f, 0.0f,
			0.0f, mScaleY, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4 AObj::getTransMat() const
	{
		//vec3 pos = CartToGL(vec3((0.5f * ( (float)mWidth ) ) + mX, (0.5f * ( (float)mHeight ) )+ mY), mGame->getCamera().getW(), mGame->getCamera().getH());

		return mat4(1.0f, 0.0f, 0.0f, mPosition(0) + (mWidth * 0.5),
			0.0f, 1.0f, 0.0f, mPosition(1) + (mHeight * 0.5),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4 AObj::getMTWMat() const
	{
		return getTransMat() * getRotMat() * getScaleMat();
	}

	AObj::~AObj()
	{

	}
}