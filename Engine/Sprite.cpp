#include "Sprite.h"
#include "Cmn.h"
#include "AnimSet.h"
#include "Texture.h"
#include "Renderer.h"
#include "Math/Operations.h"
#include <iostream>


namespace talga
{
	Sprite::Sprite(cpTex tex, I32 width, I32 height, F32 transparencyScale, UVFrame frame)
		: mTex(tex)
		, mTransparencyScale(transparencyScale)
		, mImageBox(width, height)
		, mUVCurrentFrame(frame)
	{
		if (mTex && width == -1 && height == -1)
		{
			mImageBox.setW(mTex->w());
			mImageBox.setH(mTex->h());
		}
		else
			TALGA_WARN(0, "Invalid texture was passed to sprite");

		mImageBox.updateVerts();
	}

	void Sprite::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mImageBox, mTex, mTransparencyScale, mUVCurrentFrame);
	}

	void Sprite::update(F32 dt)
	{
		mImageBox.updateVerts();
	}

	AnimSprite::AnimSprite(cpAnimSet anims, I32 width, I32 height)
		: mAnims(anims)
		, mImageBox(width, height)
		, isAnimated(true)
		, mCurrentFrame(-1)
		, mFrameSpeed(-1)
		, mTimeSince(0)
		, isLoop(false)
		, mCurrentAnimation(nullptr)
	{
		if (mAnims && mAnims->tex())
		{
			mImageBox.setW(mAnims->tex()->w());
			mImageBox.setH(mAnims->tex()->h());
		}
		else if (mAnims && !mAnims->tex())
		{
			mImageBox.setW(32);
			mImageBox.setH(32);
			TALGA_WARN(0, "Invalid texture was passed to animated sprite");
		}
			

	}

	void AnimSprite::render(Renderer* renderer, const Camera* camera) const
	{
		if (mCurrentAnimation)
			renderer->submit(mImageBox, mAnims->tex(), 1.0f, mUVCurrentFrame);
		else
			renderer->submit(mImageBox);
	}

	void AnimSprite::update(F32 dt)
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

				setUVFrame();
			}
		}

		mImageBox.updateVerts();
	}

	void AnimSprite::playAnimation(const std::string& animName, I32 speed, bool loop) // in Milliseconds
	{
		if (!isAnimated || !mAnims->tex())
			return;

		mCurrentAnimation = mAnims->getAnim(animName);

		if (!mCurrentAnimation)
			return;
		mCurrentFrame = 0;
		setUVFrame();
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

	void AnimSprite::playDefault()
	{
		playAnimation("default", 5000, true);
	}

	void AnimSprite::setUVFrame()
	{
		mUVCurrentFrame = mCurrentAnimation->at(mCurrentFrame);
	}
}