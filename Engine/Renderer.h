#pragma once

#include <stack>
#include <vector>

#include "Cmn.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"
#include "AnimSet.h"
#include "AAsset.h"
#include "Point.h"

namespace ftgl
{
	struct texture_atlas_t;
	struct texture_font_t;
}

namespace talga
{
	struct VertexData
	{
		vec3 position;
		vec4 color;
		vec2 uv;
		F32 tid;
		F32 transparencyScale;
	};

	const U32 RENDERER_MAX_ACTIVE_TEXTURES = 32;
	const U32 RENDERER_MAX_SPRITES		= 11000;
	const U32 RENDERER_VERTEX_SIZE		= sizeof(VertexData);
	const U32 RENDERER_SPRITE_SIZE		= RENDERER_VERTEX_SIZE * 4;
	const U32 RENDERER_BUFFER_SIZE		= RENDERER_MAX_SPRITES * RENDERER_SPRITE_SIZE;
	const U32 RENDERER_INDICIES_SIZE	= RENDERER_MAX_SPRITES * 6;

	const I32 SHADER_POSITION_LOC		= 0;
	const I32 SHADER_COLOR_LOC			= 1;
	const I32 SHADER_UV_LOC				= 2;
	const I32 SHADER_TID_LOC			= 3;
	const I32 SHADER_TRANSPARENCYSCALE_LOC = 4;


	class Rectangle;
	class Camera;

	class Renderer : public AAsset
	{
	public:
		Renderer(const char* vertexShaderPath, const char* fragmentShaderPath);
		~Renderer();
		
		void Init();

		void begin();
		//draw 
		void submit(const Rectangle& imageBox, cpTex texture = nullptr, F32 transparencyScale = 1.0f, UVFrame frame = UVFrame());
		void drawString(const std::string& str, vec3 pos, vec4 color);
		//draw line
		//void submit(const fPnt& pnt1, const fPnt& pnt2);
		void render();
		void end();

		virtual bool load(std::string path, AssetManager& manager) override;
		virtual bool save(std::string path, AssetManager& manager) override;
		virtual void destroy() override;

		size_t tStackSize() const { return mTransformationStack.size(); }
		void tStackPush(const mat4& mat);
		void tStackPop();

		const Camera* getCamera() const { return mCamera; }
		void setCamera(Camera* camera) { mCamera = camera; }
	protected:
		std::stack<mat4> mTransformationStack;
		std::vector<U32> mTextureSlots;
		ftgl::texture_atlas_t* mFTAtlas;
		ftgl::texture_font_t* mFTFont;

		VertexData* mNextVertex;
		Camera* mCamera;
		U32 mProgram;
		U32 mVAO;
		U32 mVBO;
		U32 mIBO;
		I32 mIndexCount;
	};

}
