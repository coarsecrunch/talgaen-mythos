#pragma once

#include <array>
#include <vector>
#include <string>
#include "Cmn.h"

#include "AAsset.h"

#include "Math/Vector2.h"
namespace talga
{
	struct Rect;

	//TL TR BR BL
	typedef std::array<vec2, 4> UVFrame;
	typedef std::vector<UVFrame> UVAnimation;
	typedef std::vector<Rect> RectList;
	typedef std::pair<std::string, UVAnimation> AnimSetPair;

	const UVFrame WHOLE_TEXTURE{ { vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0) } };

	class AnimationSet : public AAsset
	{
	public:
		AnimationSet(cpTex tex, const std::string& name);

		void addAnim(std::string name, const RectList& frames);
		const UVAnimation* getAnim(const std::string& name) const;
		cpTex tex() const { return mTex; }

		virtual bool load(std::string path, AssetManager& manager) override { return true; }
		virtual bool save(std::string path, AssetManager& manager) override { return true; }
		virtual void destroy() override {}

		size_t numAnimations() const { return mAnims.size(); }
		I32 w() const { return mWidth; }
		I32 h() const { return mHeight; }
		~AnimationSet();
	protected:
		std::vector<AnimSetPair> mAnims;
		cpTex mTex;
		I32 mWidth;
		I32 mHeight;
	};
}
