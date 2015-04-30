#include "Transformation2D.h"

namespace talga
{
	Transformation2D::Transformation2D(vec3 position, F32 orientation, F32 scaleX, F32 scaleY)
		: mPosition(position)
		, mOrientation(orientation)
		, mScaleX(scaleX)
		, mScaleY(scaleY)
	{
	}

	Transformation2D::Transformation2D(const Transformation2D& cpy)
		: mPosition(cpy.mPosition)
		, mOrientation(cpy.mOrientation)
		, mScaleX(cpy.mScaleX)
		, mScaleY(cpy.mScaleY)
	{
	}

	const Transformation2D& Transformation2D::operator=(const Transformation2D& cpy)
	{
		mPosition = cpy.mPosition;
		mOrientation = cpy.mOrientation;
		mScaleX = cpy.mScaleX;
		mScaleY = cpy.mScaleY;

		return *this;
	}


	Transformation2D::~Transformation2D()
	{
	}
}