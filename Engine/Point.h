#pragma once

#include "Cmn.h"

namespace talga
{
	class Point
	{
	public:
		Point(F32 x = 0.0f, F32 y = 0.0f) : mX(x), mY(y) {}
		
		F32 x() const { return mX; }
		F32 y() const { return mY; }
		F32 setX(F32 value) { mX = value; }
		F32 setY(F32 value) { mY = value; }

	protected:
		F32 mX;
		F32 mY;
	};
}