#pragma once

#include "Cmn.h"
#include <string>

namespace talga
{
	class Texture
	{
	public:
		Texture();
		~Texture();
		talga::I32 Init(std::string path);

		void Destroy();
		void Bind() const;

		talga::I32 id() const { return mTexture; }
		talga::I32 w() const { return mWidth; }
		talga::I32 h() const { return mHeight; }
		talga::U32 texture() const { return mTexture; }

		std::string name;
	private:
		talga::U32 mTexture;
		talga::I32 mWidth;
		talga::I32 mHeight;
	};
}