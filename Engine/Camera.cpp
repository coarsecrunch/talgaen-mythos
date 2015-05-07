#include "GL\glew.h"
#include "GLFW\glfw3.h"
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
	//glfw error callback
	void error_callback(int code, const char* message)
	{
		std::cout << "GLFW error " << code << ": " << message << std::endl;
	}

	Camera::Camera(I32 width, I32 height, vec3 position)
		: mBox(width, height, position)
		, mProjectionMatrix()
	{
		mProjectionMatrix = mat4((2.0f / (width - 0.0f)), 0.0f, 0.0f, 0.0f, // -(right + left) / (right - left)
			0.0f, (2.0f / (0.0f - height)), 0.0f, 0.0f, // -(top + bottom) / (top - bottom)
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}


	void Camera::update(F32 dt)
	{
		updateCameraScaleMat();
		updateCameraRotMat();
		updateCameraTransMat();
		updateCameraMat();
	}

	const mat4& Camera::getCameraMat() const
	{
		return mCameraMat;
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
		mCameraTransMat = mat4(1.0f, 0.0f, 0.0f, (-mBox.getX() + (-0.5 * mBox.getWidth())),
			0.0f, 1.0f, 0.0f, (-mBox.getY() + (-0.5 * mBox.getHeight())),
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