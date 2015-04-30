#pragma once

#include <vector>
#include "AObj.h"
#include "Cmn.h"
#include "Math/Vector3.h"
#include "GL\glew.h"
#include "AssetManager.h"
#include "Animation.h"

struct GLFWwindow;


namespace talga
{
	struct Rect;
	class Game;
	class ARenderableObj;
	class Animation;
	class PhysSprite;
	class AssetManager;

	class Camera : public AObj
	{
	public:
		Camera(Game* game, vec4 position = vec4(0.0f, 0.0f, 0.0f, 1.0f));
		~Camera();

		enum VAOs { TEXTURED_QUAD, COLORED_QUAD, PARTICLE_QUAD, numVAOs };
		enum Buffer_IDs { ARRAY_BUFFER_TEXTURED_QUAD, ELEMENT_BUFFER_TEXTURED_QUAD, ARRAY_BUFFER_COLLIDER_QUAD, ELEMENT_BUFFER_COLLIDER_QUAD, numBuffers };
		enum Attrib_IDs { TEXTURED_QUAD_vPosition = 0, TEXTURED_QUAD_vUvCoord = 1, COLORED_QUAD_vPosition = 0 };

		static unsigned int mVAOs[numVAOs];
		static I32 MVPShaderLoc;

		static U32 mBuffers[numBuffers];
		static U32 mProgram;

		static int InitRendering(int height, int width);

		virtual void Update(F32 dt) override;

		void Update();
		static void Clear();
		void Destroy();
		void RenderObj(const ARenderableObj& obj, const AssetManager* man) const;
		void RenderQuad(const vec4& pos, const vec4& color, const vec4& size) const;
		const mat4& getCameraMat() const;
		static GLFWwindow* mWindow;

		void setW(I32 value); //OVERRIDE
		void setH(I32 value); // OVERRIDE

		const mat4& getProjectionMatrix() const { return mProjectionMatrix; }

	protected:

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
	public:
		void RenderCollider(PhysSprite& collider);
		void setRenderCollisions(bool value) { mRenderCollisions = value; }
		bool getRenderCollisions() const { return mRenderCollisions; }

		static U32 mColliderProgram;
		static I32 mColliderModelLoc;
		static I32 mColliderViewLoc;
		static I32 mColliderProjectionLoc;
		static I32 mColorUnifromLoc;
		static I32 mBigBellyLoc;
		static I32 mREDLoc;
		bool mRenderCollisions = false;

	};
}