#pragma once

#include "Math/Matrix4x4.h"
#include "Cmn.h"
#include "Math/Vector4.h"

namespace talga
{
	class Game;

	class AObj
	{
	public:
		AObj(Game* game, I32 width = -1, I32 height = -1, vec4 position = vec4(0.0f, 0.0f, 0.0f, 1.0f));

		~AObj();
		AObj(const AObj& cpy);
		const AObj& operator=(const AObj&);

		void Destroy();
		virtual void Update(F32 dt) = 0;

		void setX(F32 value) { mPosition[0] = value; }
		void setY(F32 value) { mPosition[1] = value; }
		void setScaleX(float value) { mScaleX = value; }
		void setScaleY(float value) { mScaleY = value; }
		F32 getX() const { return mPosition(0); }
		F32 getY() const { return mPosition(1); }
		I32 getW() const { return mWidth * mScaleX; }
		I32 getH() const { return mHeight * mScaleY; }
		I32 getAW() const { return mWidth; }
		I32 getAH() const { return mHeight; }
		F32 getScaleX() const { return mScaleX; }
		F32 getScaleY() const { return mScaleY; }
		F32 getRotation() const { return mRotation; }
		void setRotation(float value) { mRotation = value; }
		Game* getGame();
		mat4 getMTWMat() const;
		vec4 getPos() const { return mPosition; }
		void setPos(vec4 value) { mPosition = value; }


		//should be protected
		void setW(I32 value) { mWidth = value; }
		void setH(I32 value) { mHeight = value; }
	protected:
		AObj() {} //SHOULD NEVER BE CALLED
		mat4 getRotMat() const;
		mat4 getScaleMat() const;
		mat4 getTransMat() const;
		vec4 mPosition;

		I32 mWidth;
		I32 mHeight;
		F32 mScaleX;
		F32 mScaleY;
		F32 mRotation;
		Game* mGame;
	};
}
