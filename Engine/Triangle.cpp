#include "Triangle.h"

namespace talga
{
	Triangle::Triangle(TriVertList list, F32 x, F32 y)
		: Transformation2D(vec3(x,y),0.0f,1.0f,1.0f)
		, mVerts( list )
	{
	}

	mat4 Triangle::getTransformationMatrix() const
	{
		return mat4::translation(vec3(mPosition[0], mPosition[1])) * mat4::scale(vec3(mScaleX, mScaleY, 1.0f)) * mat4::rotation(mOrientation);
	}

	void Triangle::updateVerts()
	{
		mat4 rotMat = mat4::rotation(mOrientation);
		mat4 scaleMat = mat4::scale(vec3(mScaleX, mScaleY, 1.0f));
		mat4 transMat = mat4::translation(vec3(mPosition[0], mPosition[1]));

		mVerts[0] = transMat * rotMat * scaleMat * vec3(mVerts[0]);
		mVerts[1] = transMat * rotMat * scaleMat * vec3(mVerts[1]);
		mVerts[2] = transMat * rotMat * scaleMat * vec3(mVerts[2]);
	}

	void Triangle::updateVertsPosition()
	{
		mVerts[0] = vec3(mVerts[0]) + mPosition;
		mVerts[1] = vec3(mVerts[0]) + mPosition;
		mVerts[2] = vec3(mVerts[1]) + mPosition;
	}

	Triangle::~Triangle()
	{
	}
}