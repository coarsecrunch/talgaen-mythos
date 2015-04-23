#include "Rectangle.h"
#include "Math/Matrix4x4.h"
namespace talga
{
	Rectangle::Rectangle(F32 width, F32 height, vec3 position, vec4 color,
		F32 orientation, F32 scaleX, F32 scaleY)
		: mPosition(position)
		, mColor(color)
		, mOrientation(orientation)
		, mScaleX(scaleX)
		, mScaleY(scaleY)
		, mWidth(width)
		, mHeight(height)
	{
		mVerts.fill(vec3());
		updateVerts();
	}

	Rectangle::Rectangle(const Rectangle& cpy)
		: mPosition(cpy.mPosition)
		, mColor(cpy.mColor)
		, mOrientation(cpy.mOrientation)
		, mScaleX(cpy.mScaleX)
		, mScaleY(cpy.mScaleY)
		, mWidth(cpy.mWidth)
		, mHeight(cpy.mHeight)
		, mVerts(cpy.mVerts)
	{
	}

	const Rectangle& Rectangle::operator=(const Rectangle& cpy)
	{
		mPosition = cpy.mPosition;
		mColor = cpy.mColor;
		mOrientation = cpy.mOrientation;
		mScaleX = cpy.mScaleX;
		mScaleY = cpy.mScaleY;
		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mVerts = cpy.mVerts;

		return *this;
	}

	void Rectangle::update(F32 dt)
	{

	}
	
	void Rectangle::updateVerts()
	{
		mat4 rotMat = mat4::rotation(mOrientation);
		mat4 scaleMat = mat4::scale(vec3(mScaleX, mScaleY, 1.0f));
		mat4 transMat = mat4::translation(vec3(mPosition[0], mPosition[1]));

		mVerts[0] = transMat * rotMat * scaleMat * vec3(-0.5f * mWidth, 0.5f * mHeight, 0);
		mVerts[1] = transMat * rotMat * scaleMat * vec3(0.5f * mWidth, 0.5f * mHeight, 0);
		mVerts[2] = transMat * rotMat * scaleMat * vec3(0.5f * mWidth, -0.5f * mHeight, 0);
		mVerts[3] = transMat * rotMat * scaleMat * vec3(-0.5f * mWidth, -0.5f * mHeight, 0);
	}


	Rectangle::~Rectangle()
	{
	}
}