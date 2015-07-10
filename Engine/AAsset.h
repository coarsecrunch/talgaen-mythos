#pragma once

#include <string>

namespace talga
{
	class AssetManager;

	class AAsset
	{
	public:
		AAsset() : mName("INVALID_ASSET_NAME"), mPath("INVALID_ASSET_PATH") {}
		AAsset(const AAsset& cpy) : mName(cpy.mName), mPath(cpy.mPath) {}
		const AAsset& operator=(const AAsset& cpy) { mName = cpy.mName; mPath = cpy.mPath; return *this; }
		virtual ~AAsset() {}

		virtual bool load(std::string path, AssetManager& manager) = 0;
		virtual bool save(std::string path, AssetManager& manager) = 0;
		virtual void destroy() = 0;

		std::string getName() const { return mName; }
    std::string getPath() const { return mPath; }
  protected:
		std::string mName;
		std::string mPath;
	};
}
