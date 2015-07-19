#pragma once
#include <vector>

#include "Cmn.h"
#include "Math/Matrix4x4.h"
#include "IRenderable.h"
#include "Renderer.h"
namespace talga
{

	class Renderer;
	class Layer
	{
	public:
		Layer(std::shared_ptr<Renderer> renderer, F32 renderWidth, F32 renderHeight);
		~Layer();

		void add(const IRenderable*);
		void remove(const IRenderable*);
		void clear();
		void render();
		std::shared_ptr<Renderer> getRenderer() { return mRenderer; }
		void setRenderer(std::shared_ptr<Renderer> renderer);
		//only call when the previous projection is on top
		void setProjectionMatrix(I32 w, I32 h);
	protected:
		F32 mWidth, mHeight;
		std::shared_ptr<Renderer> mRenderer;
		std::vector<const IRenderable*> mRenderList;
	};
}
