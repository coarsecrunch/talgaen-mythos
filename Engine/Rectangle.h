#pragma once

#include "Cmn.h"
#include "Transformation2D.h"
#include "Math/Vector4.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include <array>

namespace talga
{
	typedef std::array<vec3, 4> RectVertList;
	class LuaEngine;

	class Rectangle : public Transformation2D
	{
	public:
		virtual ~Rectangle();
		Rectangle(F32 width, F32 height, vec3 position = vec3(), F32 orientation = 0.0f, F32 scaleX = 1.0f, F32 scaleY = 1.0f);

		Rectangle(const Rectangle& cpy);
		const Rectangle& operator=(const Rectangle& cpy);

		virtual void update(F32 dt);

		const RectVertList getVerts() const { return mVerts; }

		mat4 getTransformationMatrix() const;
		void updateVerts();
		/*Only updates vertices position, completely avoids using matrix multiplication
		so much faster*/
		void updateVertsPosition();

		inline F32 getW() const { return mWidth * mScaleX; }
		inline F32 getH() const { return mHeight * mScaleY; }

		inline void setScaleX(F32 value) { mScaleX = value; updateVerts(); }
		inline void setScaleY(F32 value) { mScaleY = value; updateVerts(); }
		inline void setX(F32 value) { mPosition[0] = value; updateVertsPosition(); }
		inline void setY(F32 value) { mPosition[1] = value; updateVertsPosition(); }

		inline void setW(F32 value) { mWidth = value; updateVertsPosition(); }
		inline void setH(F32 value) { mHeight = value; updateVertsPosition(); }
		
	protected:
		bool hasChanged;
		F32 mWidth;
		F32 mHeight;
		RectVertList mVerts; // TR TL BR BL
	};
}
