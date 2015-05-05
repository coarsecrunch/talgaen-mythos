#pragma once

#include <vector>
#include "Cmn.h"
#include "IDynamic.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "AssetManager.h"
#include "Animation.h"
#include "Rectangle.h"

namespace talga
{
	struct Rect;
	class Game;
	class ARenderableObj;
	class Animation;
	class PhysSprite;
	class AssetManager;

	class Camera : public IDynamic
	{
	public:
		Camera(I32 width, I32 height, vec3 position = vec3(0.0f, 0.0f, 1.0f));
		~Camera();

		virtual void update(F32 dt) override;

		void Update();
	
		const mat4& getCameraMat() const;


		const mat4& getProjectionMatrix() const { return mProjectionMatrix; }
		Rectangle& getBox() { return mBox; }
		const Rectangle& getBox() const { return mBox; }
	protected:
		Rectangle mBox;

		mat4 mProjectionMatrix;
		mat4 mCameraMat;
		mat4 mCameraScaleMat;
		mat4 mCameraRotMat;
		mat4 mCameraTransMat;
		// Rendering globals for GL

		void updateCameraScaleMat();
		void updateCameraRotMat();
		void updateCameraTransMat();
		void updateCameraMat();

	};
}