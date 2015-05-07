#include "Layer.h"
#include "Renderer.h"
#include "IRenderable.h"
#include "Sprite.h"

namespace talga
{
	Layer::Layer(Renderer* renderer, F32 renderWidth, F32 renderHeight)
		: mRenderer(renderer)
		, mProjectionMatrix(OrthographicProjectionMat2D(renderWidth, renderHeight))
	{
        //mRenderer->push(mProjectionMatrix);
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

	Layer::~Layer()
	{
	}
}
