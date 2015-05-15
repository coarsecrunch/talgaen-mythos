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
		virtual ~Sprite() { TALGA_MSG("Sprite destructor called"); }

		virtual void render(Renderer* renderer, const Camera* camera) const override;
		virtual void update(F32 dt) override;

		Rectangle& getBox() { return mImageBox; }

	protected:
		Rectangle mImageBox;
		cpTex mTex;
		UVFrame mUVCurrentFrame;
	};

	class AnimSprite : public IRenderable, public IDynamic
	{
	public:
		AnimSprite(cpAnimSet anims, I32 width = -1, I32 height = -1);
		virtual ~AnimSprite() { TALGA_MSG("Sprite destructor called"); };

		virtual void render(Renderer* renderer, const Camera* camera) const override;
		virtual void update(F32 dt) override;

		/*speed in ms*/
		void playAnimation(const std::string& animName, I32 speed, bool loop);
		void playDefault();

		Rectangle& getBox() { return mImageBox; }

		
	protected:
		void setUVFrame();
		Rectangle mImageBox;
		UVFrame mUVCurrentFrame;

		// Animation specific members

		cpAnimSet mAnims;
		const UVAnimation* mCurrentAnimation;
		bool isAnimated;
		bool isLoop;
		F32 mTimeSince;
		F32 mFrameSpeed;
		I32 mCurrentFrame;
	};
}