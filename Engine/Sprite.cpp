#include "Sprite.h"
#include "Cmn.h"
#include "AnimSet.h"
#include "Texture.h"
#include "Renderer.h"
#include "Math/Operations.h"
#include <iostream>


namespace talga
{
	Sprite::Sprite(cpTex tex, UVFrame quords, F32 transScale)
		: mTex(tex)
		, mTransparencyScale(transScale)
		, mUVCurrentFrame(quords)
	{
		TALGA_WARN(tex, "invalid texture passed to Sprite")

    if (tex)
    {
      box().setW(abs(quords[1].x() - quords[0].x()) * tex->w());
	  box().setH(abs(quords[3].y() - quords[0].y()) * tex->h());
    }
    else
    {
      box().setW(64);
      box().setH(64);
    }

		box().updateVerts();
	}

	void Sprite::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(box(), mTex, mTransparencyScale, mUVCurrentFrame);
		IRenderable::render(renderer, camera);
	}

	void Sprite::update(F32 dt)
	{
		box().updateVerts();
	}

	AnimSprite::AnimSprite(cpAnimSet anims, I32 width, I32 height)
		: mAnims(anims)
		, isAnimated(true)
		, mCurrentFrame(-1)
		, mFrameSpeed(-1)
		, mTimeSince(0)
		, isLoop(false)
		, mCurrentAnimation(nullptr)
	{
		box().setW(anims->w());
		box().setH(anims->h());
	}

	void AnimSprite::render(Renderer* renderer, const Camera* camera) const
	{
		if (mCurrentAnimation)
			renderer->submit(box(), mAnims->tex(), 1.0f, mUVCurrentFrame);
		else
			renderer->submit(box());

		IRenderable::render(renderer, camera);
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
					playAnimation("default", 1000, true);
					mTimeSince = 0;
				}

				setUVFrame();
			}
		}

		box().updateVerts();
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
		box().setW((mCurrentAnimation->at(mCurrentFrame).at(1)[0] - mCurrentAnimation->at(mCurrentFrame).at(0)[0])
			* mAnims->tex()->w());
		box().setH((mCurrentAnimation->at(mCurrentFrame).at(0)[1] - mCurrentAnimation->at(mCurrentFrame).at(3)[1])
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
