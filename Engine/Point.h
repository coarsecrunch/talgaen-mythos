#pragma once

#include "Cmn.h"

namespace talga
{
template<typename T>
	class Point
	{
	public:
        Point(T x = 0, T y = 0) : mX(x), mY(y) {}
		
        T x() const { return mX; }
        T y() const { return mY; }
        void setX(T value) { mX = value; }
        void setY(T value) { mY = value; }

	protected:
        T mX;
        T mY;
	};

    typedef Point<F32> fPnt;
    typedef Point<I32> iPnt;
}
