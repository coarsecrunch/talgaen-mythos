#include "Sprite.h"
#include "Cmn.h"
#include "AnimSet.h"
#include "Texture.h"
#include "Renderer.h"
#include "Math/Operations.h"
#include <iostream>


namespace talga
{
	Sprite::Sprite(cpTex tex)
		: mTex(tex)
		, mTransparencyScale(1.0f)
		, mImageBox(tex->w(), tex->h())
		, mUVCurrentFrame(DEFAULT_SPRITE_UV)
	{
		TALGA_WARN(tex, "invalid texture passed to Sprite")

		mImageBox.updateVerts();
	}

	void Sprite::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mImageBox, mTex, mTransparencyScale, mUVCurrentFrame);
		for (auto it = getChildren().begin(); it != getChildren().end(); ++it)
		{
			renderer->tStackPush(mImageBox.getTransformationMatrix());
			(*it)->render(renderer, camera);
			renderer->tStackPop();
		}
	}

	void Sprite::update(F32 dt)
	{
		mImageBox.updateVerts();
	}

	AnimSprite::AnimSprite(cpAnimSet anims, I32 width, I32 height)
		: mAnims(anims)
		, mImageBox(anims->w(), anims->h())
		, isAnimated(true)
		, mCurrentFrame(-1)
		, mFrameSpeed(-1)
		, mTimeSince(0)
		, isLoop(false)
		, mCurrentAnimation(nullptr)
	{
	}

	void AnimSprite::render(Renderer* renderer, const Camera* camera) const
	{
		if (mCurrentAnimation)
			renderer->submit(mImageBox, mAnims->tex(), 1.0f, mUVCurrentFrame);
		else
			renderer->submit(mImageBox);

		for (auto it = getChildren().begin(); it != getChildren().end(); ++it)
		{
			renderer->tStackPush(mImageBox.getTransformationMatrix());
			(*it)->render(renderer, camera);
			renderer->tStackPop();
		}
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
		mImageBox.setW((mCurrentAnimation->at(mCurrentFrame).at(1)[0] - mCurrentAnimation->at(mCurrentFrame).at(0)[0])
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