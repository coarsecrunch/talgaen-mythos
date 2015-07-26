#pragma once

#include <array>

#include "Transformation2D.h"
#include "Math/Vector2.h"
#include "Math/Matrix4x4.h"
namespace talga
{
	typedef std::array <vec3, 3>  TriVertList;
	class Triangle : public Transformation2D
	{
	public:
		Triangle(TriVertList list, F32 x = 0.0f, F32 y = 0.0f);
		~Triangle();

		mat4 getTransformationMatrix() const;
		void updateVerts();
		/*Only updates vertices position, completely avoids using matrix multiplication
		so much faster*/

		const TriVertList& getVerts() const { return outputVerts; }
		TriVertList& getRealVerts() { return mVerts; }
		TriVertList& getVerts() { return outputVerts; }
		void updateVertsPosition();

	private:
		TriVertList mVerts;
		TriVertList outputVerts;
	};
}
