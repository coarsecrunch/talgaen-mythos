#pragma once
#include "Rectangle.h"
#include "IRenderable.h"
#include "Cmn.h"

namespace talga
{
	class Sprite : public IRenderable
	{
	public:
		Sprite(cpTex tex, I32 width, I32 height);
		~Sprite();

		virtual void render(Renderer* renderer) const override;
	protected:
		Rectangle mImageBox;
		cpTex mTex;
	};
}