#pragma once

#include "Cmn.h"
#include "Math/Matrix4x4.h"
#include "IRenderable.h"
#include <vector>

namespace talga
{

	class Renderer;
	class Layer
	{
	public:
		Layer(Renderer* renderer, F32 renderWidth, F32 renderHeight);
		~Layer();

		void add(const IRenderable* sprite);
		
		void render();
		Renderer* getRenderer() { return mRenderer; }
	protected:
		Renderer* mRenderer;
		mat4 mProjectionMatrix;
		std::vector<const IRenderable*> mRenderList;
	};
}