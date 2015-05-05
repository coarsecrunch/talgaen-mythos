#pragma once

#include "Cmn.h"
#include <string>

namespace talga
{
	class Texture
	{
	public:
		Texture(std::string path);
		~Texture();
		I32 Init(std::string path);
		
		void destroy();

		void Bind() const;

		I32 id() const { return mTexture; }
		I32 w() const { return mWidth; }
		I32 h() const { return mHeight; }
		U32 texture() const { return mTexture; }


		std::string name;
	private:
		talga::U32 mTexture;
		talga::I32 mWidth;
		talga::I32 mHeight;
	};
}