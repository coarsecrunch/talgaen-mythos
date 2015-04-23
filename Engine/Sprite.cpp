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
		, mUVCurrentFrame(UVFrame({ { vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0) } }))
		, mCurrentFrame(-1)
		, mFrameSpeed(-1)
		, mTimeSince(0)
		, isLoop(false)
		, mCurrentAnimation(nullptr)

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
		, mCurrentFrame(-1)
		, mFrameSpeed(-1)
		, mTimeSince(0)
		, isLoop(false)
		, mCurrentAnimation(nullptr)
	{
		if (mAnims)
		{
			mImageBox.setW(mAnims->tex()->w());
			mImageBox.setH(mAnims->tex()->h());
		}
		else
			std::cerr << "Invalid animation was passed to sprite" << std::endl;

	}

	void Sprite::render(Renderer* renderer) const
	{
		if (isAnimated)
		{
			if (mCurrentAnimation)
				renderer->submit(mImageBox, mAnims->tex(), mCurrentAnimation->at(mCurrentFrame));
		}
		else
			renderer->submit(mImageBox, mTex, mUVCurrentFrame);
	}

	void Sprite::update(F32 dt)
	{
		if (isAnimated)
		{
			mTimeSince += dt;

			if (mCurrentAnimation)
			{
				mCurrentFrame = mTimeSince / mFrameSpeed;

				if ((mCurrentFrame >= mCurrentAnimation->size()) && isLoop)
				{
					mCurrentFrame = 0;
					mTimeSince = 0;
				}
				else if (mCurrentFrame >= mCurrentAnimation->size() && !isLoop)
				{
					mCurrentFrame = 0;
					playAnimation("default", 5000, true);
					mTimeSince = 0;
				}
			}
		}

		mImageBox.updateVerts();
	}

	void Sprite::playAnimation(const std::string& animName, I32 speed, bool loop) // in Milliseconds
	{
		if (!isAnimated)
			return;

		mCurrentAnimation = mAnims->getAnim(animName);

		if (!mCurrentAnimation)
			return;
		mCurrentFrame = 0;
		isLoop = loop;
		mFrameSpeed = speed / mCurrentAnimation->size();
		mTimeSince = 0;

		// should probably be taken out, and just have a seperate array of
		// rectangles with pixel coords in the animation
		mImageBox.setW((mCurrentAnimation->at(mCurrentFrame).at(1)[0] - mCurrentAnimation->at(mCurrentFrame).at(0)[0] )
			* mAnims->tex()->w());
		mImageBox.setH((mCurrentAnimation->at(mCurrentFrame).at(0)[1] - mCurrentAnimation->at(mCurrentFrame).at(3)[1])
			* mAnims->tex()->h());
	}

	void Sprite::playDefault()
	{
		playAnimation("default", 5000, true);
	}

	Sprite::~Sprite()
	{
	}
}