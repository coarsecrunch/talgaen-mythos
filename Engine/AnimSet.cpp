#include "AnimSet.h"
#include <iostream>
#include "Rect.h"
#include "Texture.h"

namespace talga
{
	static vec2 GET_UV(F32 x, F32 xMax, F32 y, F32 yMax)
	{
		return vec2(x / xMax, -(y / yMax));
	}

	AnimationSet::AnimationSet(cpTex tex)
		: mTex(tex)
		, mAnims({AnimSetPair("default",
	 UVAnimation{ UVFrame{ { vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0) } } }) })
	{
		// Sweet baby talga this is ridiculous
	}

	const UVAnimation* AnimationSet::getAnim(const std::string& name) const
	{
		for (auto iter = mAnims.begin(); iter != mAnims.end(); ++iter)
			if ((*iter).first == name)
				return &(*iter).second;

		std::cout << "Could not find animation: " << name << std::endl;
		return nullptr;
	}

	/* Takes a list of Rectangles using pixel coordinates and converts them
	to UV cooridnates and stores them in the Animation set*/

	void AnimationSet::add(std::string name, const RectList& frames)
	{
		UVAnimation anim;
		for (const auto& rectFrame : frames)
		{
			UVFrame frame;
			
			frame[0] = GET_UV(rectFrame.x, mTex->w(), rectFrame.y, mTex->h());
			frame[1] = GET_UV(rectFrame.x + rectFrame.w, mTex->w(), rectFrame.y, mTex->h());
			frame[2] = GET_UV(rectFrame.x + rectFrame.w, mTex->w(), rectFrame.y + rectFrame.h, mTex->h());
			frame[3] = GET_UV(rectFrame.x, mTex->w(), rectFrame.y + rectFrame.h, mTex->h());

			anim.push_back(frame);
		}

		mAnims.push_back(AnimSetPair(name, anim));
	}

	AnimationSet::~AnimationSet()
	{

	}
}