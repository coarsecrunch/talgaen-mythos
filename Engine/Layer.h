#pragma once

#include "Cmn.h"
#include "Math/Matrix4x4.h"
#include "IRenderable.h"
#include <vector>
#include "Renderer.h"
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
		void setRenderer(Renderer* renderer);
		//only call when the previous projection is on top
		void setProjectionMatrix(I32 w, I32 h);
	protected:
		F32 mWidth, mHeight;
		Renderer* mRenderer;
		std::vector<const IRenderable*> mRenderList;
	};
}
