#pragma once
#include "Rectangle.h"
#include "Math\Vector2.h"
#include "IRenderable.h"
#include "IDynamic.h"
#include "AnimSet.h"
#include "Cmn.h"

namespace talga
{
	class Sprite : public IRenderable, public IDynamic
	{
	public:
		Sprite(cpTex tex, I32 width = -1, I32 height = -1);
		Sprite(cpAnimSet anims, I32 width = -1, I32 height = -1);
		Sprite(cpAnimSet anims);
		~Sprite();

		virtual void render(Renderer* renderer) const override;
		virtual void update(F32 dt) override;
	protected:
		Rectangle mImageBox;
		cpTex mTex;

		cpAnimSet mAnims;
		UVFrame mCurrentFrame;
		const UVAnimation* mCurrentAnimation;
		bool isAnimated;
	};
}