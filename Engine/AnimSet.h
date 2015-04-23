#pragma once

#include <array>
#include <vector>
#include <string>
#include "Cmn.h"

#include "Math/Vector2.h"

namespace talga
{
	struct Rect;

	//TL TR BR BL
	typedef std::array<vec2, 4> UVFrame;
	typedef std::vector<UVFrame> UVAnimation;
	typedef std::vector<Rect> RectList;
	typedef std::pair<std::string, UVAnimation> AnimSetPair;

	class AnimationSet
	{
	public:
		AnimationSet(cpTex tex);

		void addAnim(std::string name, const RectList& frames);
		const UVAnimation* getAnim(const std::string& name) const;
		cpTex tex() const { return mTex; }

		const std::string& getName() const { return mName; }
		void setName(std::string value) { mName = value; }
		I32 numAnimations() const { return mAnims.size(); }

		~AnimationSet();
	protected:
		std::vector<AnimSetPair> mAnims;
		std::string mName;
		cpTex mTex;
	};
}