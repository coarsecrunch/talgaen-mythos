#pragma once

#include "Vector3.h"
#include "Cmn.h"

namespace talga
{
	class Transformation2D
	{
	public:
		Transformation2D(vec3 position, F32 orientation, F32 scaleX, F32 scaleY);
		~Transformation2D();

		Transformation2D(const Transformation2D& cpy);
		const Transformation2D& operator=(const Transformation2D& cpy);
	protected:
		vec3 mPosition;
		F32 mScaleX;
		F32 mScaleY;
		F32 mOrientation;
	};
}
