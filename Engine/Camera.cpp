#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Game.h"
#include "Map.h"
#include "Rect.h"
#include "Rendering.h"
#include <string>
#include <iostream>
#include "Math/Matrix3x3.h"
#include "AssetManager.h"

namespace talga
{
	Camera::Camera(I32 width, I32 height, vec3 position)
		: mBox(width, height, position)
	{
		update(0);
	}


	void Camera::update(F32 dt)
	{
		updateCameraScaleMat();
		updateCameraRotMat();
		updateCameraTransMat();
		updateCameraMat();
	}

    void Camera::setW(I32 w)
    {
        mBox.setW(w);
        update(0);
    }

    void Camera::setH(I32 h)
    {
        mBox.setH(h);
        update(0);
    }

	const mat4& Camera::getCameraMat() const
	{
		return mCameraMat;
	}

	vec3 Camera::screenToWorld(const vec3& pos)
	{
		mat4 inversePosition(
			1.0f, 0.0f, 0.0f, mBox.getX() - (mBox.getW() * 0.5f),
			0.0f, 1.0f, 0.0f, mBox.getY() - (mBox.getH() * 0.5f),
			0.0f, 0.0f, 1.0f, mBox.getZ(),
			0.0f, 0.0f, 0.0f, 1.0f);

		mat4 inverseScale(
			mBox.getScaleX(), 0.0f, 0.0f, 0.0f,
			0.0f, mBox.getScaleY(), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return  inversePosition * Transpose(mCameraRotMat) *  inverseScale * pos;
	}

	void Camera::updateCameraScaleMat()
	{
		mCameraScaleMat = mat4((1.0f / mBox.getScaleX()), 0.0f, 0.0f, 0.0f,
			0.0f, (1.0f / mBox.getScaleY()), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Camera::updateCameraRotMat()
	{
		mCameraRotMat = mat4(cos(mBox.getOrientation()), sin(mBox.getOrientation()), 0.0f, 0.0f,
			-sin(mBox.getOrientation()), cos(mBox.getOrientation()), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	void Camera::updateCameraTransMat()
	{
		mCameraTransMat = mat4(1.0f, 0.0f, 0.0f, (-mBox.getX()),
			0.0f, 1.0f, 0.0f, (-mBox.getY()),
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Camera::updateCameraMat()
	{
		mCameraMat = mCameraScaleMat * mCameraRotMat * mCameraTransMat;
	}

	Camera::~Camera()
	{
	}

}
