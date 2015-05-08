#include "Layer.h"
#include "Renderer.h"
#include "IRenderable.h"
#include "Sprite.h"

namespace talga
{
	Layer::Layer(Renderer* renderer, F32 renderWidth, F32 renderHeight)
		: mRenderer(renderer)
		, mWidth{ renderWidth }
		, mHeight{ renderHeight }
	{
		setProjectionMatrix(mWidth, mHeight);
	}

	void Layer::add(const IRenderable* sprite)
	{
		mRenderList.push_back(sprite);
	}

	void Layer::render()
	{
		mRenderer->begin();
		for (const IRenderable* rdr : mRenderList)
        {
            const Sprite* spr = (const Sprite*) rdr;
			rdr->render(mRenderer, mRenderer->getCamera());
		}
		mRenderer->end();
		mRenderer->render();
	}

	void Layer::setProjectionMatrix(I32 w, I32 h)
	{	
		while (mRenderer->tStackSize() > 1)
			mRenderer->tStackPop();
		
		mRenderer->tStackPush(OrthographicProjectionMat2D(w, h));
	}

	void Layer::setRenderer(Renderer* renderer)
	{ 
		mRenderer = renderer;
		setProjectionMatrix(mWidth, mHeight);
	}

	Layer::~Layer()
	{
	}
}
