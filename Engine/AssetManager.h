#pragma once

#include <string>
#include <vector>
#include "Cmn.h"
namespace talga
{
	class AnimationSet;
	class Texture;
	class Map;
	class Font;
	class AAsset;
	struct Rect;


	class AssetManager
	{
	public:
		static const U32 MAX_ASSETS = 10000;
		AssetManager();
		~AssetManager();

		cpTex AddTexture(std::string path);
		void AddAnimationSet(std::string name, const AnimationSet& anim);
		cpMap AddMap(std::string path);
		cpTex GetTexture(std::string name) const;
		cpMap GetMap(std::string name) const;

		const std::vector<Texture>& getTextures() const { return mTextures; }
		const std::vector<Map>& getMaps() const { return mMaps; }

		cpFont addFont(const std::string& path, I32 size);
		cpFont getFont(const std::string& path) const;

		static cpTex NO_TEXTURE;
	private:
		const AAsset* assetExists(const std::string& name) const;


		std::vector<Texture> mTextures;
		std::vector<AnimationSet> mAnimationSets;
		std::vector<Map> mMaps;
		std::vector<Font> mFonts;
	};
}
