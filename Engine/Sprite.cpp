#include "Sprite.h"
#include "Texture.h"
#include "Renderer.h"

namespace talga
{
	Sprite::Sprite(cpTex tex, I32 width, I32 height)
		: mTex(tex)
		, mImageBox(width, height)
	{
		if (tex)
		{
			mImageBox.setW(tex->w());
			mImageBox.setY(tex->h());
		}

	}

	void Sprite::render(Renderer* renderer) const
	{
		renderer->submit(mImageBox, mTex);
	}


	Sprite::~Sprite()
	{
	}
}