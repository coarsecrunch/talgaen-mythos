#pragma once

#include "AObj.h"
#include "Cmn.h"
#include <vector>

namespace talga
{

	class Camera;
	class AssetManager;

	class ARenderableObj : public AObj
	{
	public:
		ARenderableObj(Game* game, std::vector<cpAnim> animations = std::vector<cpAnim>(), vec4 position = vec4(0.0f, 0.0f, 0.0f, 1.0f), I32 width = -1, I32 height = -1);

		ARenderableObj(const ARenderableObj& cpy);
		const ARenderableObj& operator=(const ARenderableObj&);
		~ARenderableObj();

		virtual void Update(F32 dt) override;
		virtual void Render(const Camera& camera) = 0;

		cpAnim getAnimation() const { return mCurrentAnimation; }
		int getFrame() const { return mCurrentFrame; }
		void setFrame(int value);
		void PlayAnimation(int index, int speed, bool loop);

		bool isVisible() const { return mVisible; }
		void setVisible(bool value) { mVisible = value; }

	protected:
		ARenderableObj() {} //SHOULD NEVER BE CALLED
		std::vector<cpAnim> mAnimations;
		cpAnim mCurrentAnimation;
		cpAnim mDefaultAnimation;
		int mCurrentFrame;
		int mFrameSpeed;
		int mTimeSince;
		bool mLoop;
		bool mVisible;
	};
}
