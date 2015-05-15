#pragma once

#include <string>

namespace talga
{
	class AssetManager;

	class AAsset
	{
	public:
		AAsset() : mName("INVALID_ASSET_NAME") {}
		AAsset(const AAsset& cpy) : mName(cpy.mName) {}
		const AAsset& operator=(const AAsset& cpy) { mName = cpy.mName; return *this; }
		virtual ~AAsset() {}

		virtual bool load(std::string path, AssetManager& manager) = 0;
		virtual void destroy() = 0;

		std::string getName() const { return mName; }
	protected:
		std::string mName;
	};
}