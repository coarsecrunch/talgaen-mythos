#include "Triangle.h"

namespace talga
{
	Triangle::Triangle(TriVertList list, F32 x, F32 y)
		: Transformation2D(vec3(x,y), 0.0f, 1.0f, 1.0f)
		, mVerts( list )
		, outputVerts()
	{
    updateVertsPosition();
	}

	mat4 Triangle::getTransformationMatrix() const
	{
		return mat4::translation(mPosition) * mat4::scale(vec3(mScaleX, mScaleY, 1.0f)) * mat4::rotation(mOrientation);
	}

	void Triangle::updateVerts()
	{
		mat4 rotMat = mat4::rotation(mOrientation);
		mat4 scaleMat = mat4::scale(vec3(mScaleX, mScaleY, 1.0f));
		mat4 transMat = mat4::translation(mPosition);

		outputVerts[0] = transMat * rotMat * scaleMat * mVerts[0];
		outputVerts[1] = transMat * rotMat * scaleMat * mVerts[1];
		outputVerts[2] = transMat * rotMat * scaleMat * mVerts[2];
	}

	void Triangle::updateVertsPosition()
	{
		outputVerts[0] = mVerts[0] + mPosition;
		outputVerts[1] = mVerts[1] + mPosition;
		outputVerts[2] = mVerts[2] + mPosition;
	}

	Triangle::~Triangle()
	{
	}
}
