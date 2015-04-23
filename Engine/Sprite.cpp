#include "Sprite.h"
#include "Cmn.h"
#include "AnimSet.h"
#include "Texture.h"
#include "Renderer.h"

#include <iostream>


namespace talga
{
	Sprite::Sprite(cpTex tex, I32 width, I32 height)
		: mTex(tex)
		, mImageBox(width, height)
		, mAnims(nullptr)
		, isAnimated(false)
		, mCurrentFrame(UVFrame({ { vec2(0, 1), vec2(1,1), vec2(1,0), vec2(0,0) } }))
	{
		if (mTex)
		{
			mImageBox.setW(mTex->w());
			mImageBox.setH(mTex->h());
		}
		else
			std::cerr << "Invalid texture was passed to sprite" << std::endl;
	}

	Sprite::Sprite(cpAnimSet anims, I32 width, I32 height)
		: mAnims(anims)
		, mImageBox(width, height)
		, mTex(nullptr)
		, isAnimated(true)
	{
		if (mAnims)
		{
			mImageBox.setW(mAnims->tex()->w());
			mImageBox.setH(mAnims->tex()->h());
			mCurrentFrame = mCurrentAnimation[0][0];
		}
		else
			std::cerr << "Invalid animation was passed to sprite" << std::endl;

	}

	void Sprite::render(Renderer* renderer) const
	{
		if (isAnimated)
			renderer->submit(mImageBox, mAnims->tex());
		else
			renderer->submit(mImageBox, mTex, mCurrentFrame);
	}

	void Sprite::update(F32 dt)
	{
		if (isAnimated)
		{

		}
	}


	Sprite::~Sprite()
	{
	}
}