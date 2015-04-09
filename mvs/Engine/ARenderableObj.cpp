#include "ARenderableObj.h"
#include "Animation.h"
#include "Rect.h"

namespace talga
{
	talga::ARenderableObj::ARenderableObj(Game* game, std::vector<cpAnim> animations, vec4 position, I32 width, I32 height)
		: AObj(game, width, height, position)
		, mCurrentAnimation(nullptr)
		, mCurrentFrame(-1)
		, mTimeSince(0)
		, mVisible(true)
		, mAnimations(animations)
	{
		mCurrentAnimation = nullptr;
		mDefaultAnimation = nullptr;
	}

	ARenderableObj::ARenderableObj(const ARenderableObj& cpy)
		: AObj(cpy)
		, mCurrentAnimation(cpy.mCurrentAnimation)
		, mCurrentFrame(cpy.mCurrentFrame)
		, mTimeSince(cpy.mTimeSince)
		, mVisible(cpy.mVisible)
		, mAnimations(cpy.mAnimations)
	{
	}

	const ARenderableObj& ARenderableObj::operator=(const ARenderableObj& cpy)
	{
		AObj::operator=(cpy);
		mAnimations = cpy.mAnimations;
		mCurrentAnimation = cpy.mCurrentAnimation;
		mDefaultAnimation = cpy.mDefaultAnimation;
		mCurrentFrame = cpy.mCurrentFrame;
		mFrameSpeed = cpy.mFrameSpeed;
		mTimeSince = cpy.mTimeSince;
		mLoop = cpy.mLoop;
		mVisible = cpy.mVisible;

		return *this;
	}

	void ARenderableObj::setFrame(int value)
	{
		mCurrentFrame = value;
		setW(mCurrentAnimation->frames[value].w);
		setH(mCurrentAnimation->frames[value].h);
	}

	void ARenderableObj::Update(F32 dt)
	{
		mTimeSince += dt;

		if (mCurrentAnimation)
		{
			mCurrentFrame = mTimeSince / mFrameSpeed;

			if ((mCurrentFrame >= mCurrentAnimation->numFrames) && mLoop)
			{
				mCurrentFrame = 0;
				mTimeSince = 0;
			}
			else if (mCurrentFrame >= mCurrentAnimation->numFrames && !mLoop)
			{
				mCurrentFrame = 0;
				PlayAnimation(0, 5000, true);
				mTimeSince = 0;
			}
		}
	}

	void ARenderableObj::PlayAnimation(int index, int speed, bool loop) // in Milliseconds
	{
		if (index < 0 || index > mAnimations.size() - 1)
			return;
		mCurrentAnimation = mAnimations[index];
		mLoop = loop;
		mFrameSpeed = speed / mCurrentAnimation->numFrames;
		mTimeSince = 0;
		setFrame(0);
	}

	ARenderableObj::~ARenderableObj()
	{
	}
}