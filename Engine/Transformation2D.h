#pragma once

#include "Math/Vector3.h"
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

		inline const vec3& getPosition() const { return mPosition; }
		inline F32 getOrientation() const { return mOrientation; }
		inline F32 getScaleX() const { return mScaleX; }
		inline F32 getScaleY() const { return mScaleY; }

		inline F32 getX() const { return mPosition(0); }
		inline F32 getY() const { return mPosition(1); }
		inline F32 getZ() const { return mPosition(2); }
		

		inline void setPosition(const vec3& value) { mPosition = value; }
		inline void setOrientation(F32 value) { mOrientation = value; }
		

		inline void setX(F32 value) { mPosition[0] = value; }
		inline void setY(F32 value) { mPosition[1] = value; }
		inline void setZ(F32 value) { mPosition[2] = value; }
	protected:
		vec3 mPosition;
		F32 mScaleX;
		F32 mScaleY;
		F32 mOrientation;
	};
}
