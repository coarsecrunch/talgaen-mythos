#pragma once

#include "Cmn.h"
#include "AAsset.h"

namespace talga
{
	class AssetManager;

	class Texture : public AAsset
	{
	public:
		Texture();
		virtual ~Texture();

		virtual void load(std::string path, AssetManager& manager) override;
		virtual void destroy() override;

		void Bind() const;

		I32 id() const { return mTexture; }
		I32 w() const { return mWidth; }
		I32 h() const { return mHeight; }
		U32 texture() const { return mTexture; }

	private:
		talga::U32 mTexture;
		talga::I32 mWidth;
		talga::I32 mHeight;
	};
}