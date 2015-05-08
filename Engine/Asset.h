#pragma once

#include <string>

#define RELATIVE_ASSETS_PATH std::string("../assets/")

namespace talga
{
	class AssetManager;

	class AAsset
	{
	public:

		virtual void load(std::string path, AssetManager& man) = 0;
		virtual void destroy() = 0;

		AAsset() : mName( "INAVLID_ASSET_NAME" ) {}
		AAsset(const AAsset& cpy) : mName(cpy.mName) {}
		const AAsset& operator=(const AAsset& cpy) { mName = cpy.mName; return *this; }

		virtual ~AAsset() {}

		std::string getName() const { return mName; }
	protected:
		std::string mName;
	};

}