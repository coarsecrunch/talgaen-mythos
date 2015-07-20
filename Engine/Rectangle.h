#pragma once

#include "Cmn.h"
#include "Transformation2D.h"
#include "Math/Vector4.h"
#include "Math/Vector3.h"
#include <array>

namespace talga
{
	typedef std::array<vec3, 4> VertList;
	class LuaEngine;

	class Rectangle : public Transformation2D
	{
	public:
		virtual ~Rectangle();
		Rectangle(F32 width, F32 height, vec3 position = vec3(), vec4 color = vec4(0.0f, 1.0f, 1.0f, 1.0f), F32 orientation = 0.0f, F32 scaleX = 1.0f, F32 scaleY = 1.0f);

		Rectangle(const Rectangle& cpy);
		const Rectangle& operator=(const Rectangle& cpy);

		virtual void update(F32 dt);

		F32 getWidth() const { return mWidth; }
		F32 getHeight() const { return mHeight; }

		const VertList getVerts() const { return mVerts; }


		void updateVerts();
		/*Only updates vertices position, completely avoids using matrix multiplication
		so much faster*/
		void updateVertsPosition();

		inline const vec3& getPosition() const { return mPosition; }
		inline const vec4& getColor() const { return mColor; }
		inline F32 getOrientation() const { return mOrientation; }
		inline F32 getScaleX() const { return mScaleX; }
		inline F32 getScaleY() const { return mScaleY; }

		inline F32 getX() const { return mPosition(0); }
		inline F32 getY() const { return mPosition(1); }
		inline F32 getZ() const { return mPosition(2); }
    inline F32 getW() const { return mWidth * mScaleX; }
    inline F32 getH() const { return mHeight * mScaleY; }

		inline void setPosition(const vec3& value) { mPosition = value; }
		inline void setColor(const vec4& value) { mColor = value; }
		inline void setOrientation(F32 value) { mOrientation = value; }
		inline void setScaleX(F32 value) { mScaleX = value; }
		inline void setScaleY(F32 value) { mScaleY = value; }

		inline void setX(F32 value) { mPosition[0] = value; }
		inline void setY(F32 value) { mPosition[1] = value; }
		inline void setZ(F32 value) { mPosition[2] = value; }
		inline void setW(F32 value) { mWidth = value; }
		inline void setH(F32 value) { mHeight = value; }

	protected:
		vec4 mColor;
		
		F32 mWidth;
		F32 mHeight;
		VertList mVerts; // TR TL BR BL
	};
}
