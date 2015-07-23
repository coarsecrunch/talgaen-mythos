#include "Layer.h"
#include "Renderer.h"
#include "IRenderable.h"
#include "Sprite.h"
#include "Triangle.h"
namespace talga
{
	/*LAYER TAKES RESPONSIBLITY FOR ALL */
	Layer::Layer(std::shared_ptr<Renderer> renderer, F32 renderWidth, F32 renderHeight)
		: mRenderer(renderer)
		, mWidth{ renderWidth }
		, mHeight{ renderHeight }
	{
        setProjectionMatrix(renderWidth, renderHeight);
	}

	void Layer::add(const IRenderable* sprite)
	{
		mRenderList.push_back(sprite);
	}

	void Layer::remove(const IRenderable* sprite)
	{
		for (const IRenderable* spr : mRenderList)
		{
			if (sprite == spr)
			{
				spr = mRenderList.back();
				mRenderList.pop_back();
				return;
			}
		}

		TALGA_WARN(0, "Failed to remove rendereable from layer");
		return;
	}

	void Layer::clear()
	{
		mRenderList.clear();
	}

	void Layer::render()
	{
		mRenderer->begin();
		for (const IRenderable* rdr : mRenderList)
		{
			if (!rdr) continue;
			rdr->render(mRenderer.get(), mRenderer->getCamera());
		}

		mRenderer->end();
		mRenderer->render();
	}

	void Layer::setProjectionMatrix(I32 w, I32 h)
	{	
        if (!mRenderer) return;

		while (mRenderer->tStackSize() > 1)
			mRenderer->tStackPop();
		
		mRenderer->tStackPush(OrthographicProjectionMat2D(w, h));
	}

	void Layer::setRenderer(std::shared_ptr<Renderer> renderer)
    {
		mRenderer = renderer;
		setProjectionMatrix(mWidth, mHeight);
	}

	Layer::~Layer()
	{
	}
}
