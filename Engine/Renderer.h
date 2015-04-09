#pragma once

#include <vector>
#include "Cmn.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"

class ARenderableObj;

namespace talga
{
	struct VertexData
	{
		vec4 position;
		vec4 color;
		vec2 uv;
	};

	const U32 RENDERER_MAX_SPRITES		= 10000;
	const U32 RENDERER_VERTEX_SIZE		= sizeof(VertexData);
	const U32 RENDERER_SPRITE_SIZE		= RENDERER_VERTEX_SIZE * 4;
	const U32 RENDERER_BUFFER_SIZE		= RENDERER_MAX_SPRITES * RENDERER_SPRITE_SIZE;
	const U32 RENDERER_INDICIES_SIZE	= RENDERER_MAX_SPRITES * 6;

	const I32 SHADER_POSITION_LOC		= 0;
	const I32 SHADER_COLOR_LOC			= 1;
	const I32 SHADER_UV_LOC				= 2;


	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		
		void Init();

		void begin();
		void submit(const ARenderableObj& obj);
		void render();
		void end();

	protected:
		VertexData* mNextVertex;
		U32  mVAO;
		U32 mVBO;
		U32 mIBO;
		I32 mIndexCount;
	};

}