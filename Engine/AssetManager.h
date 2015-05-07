#pragma once

#include <string>
#include <vector>
#include "Cmn.h"

namespace talga
{
	class AnimationSet;
	class Texture;
	class Map;
	struct Rect;

	class AssetManager
	{
	public:
		static const unsigned int MAX_ASSETS = 10000;

		AssetManager();
		~AssetManager();

		void AddTexture(std::string path);
		void AddAnimationSet(std::string name, const AnimationSet& anim);
		void LoadMap(std::string path);
		cpTex GetTexture(std::string name) const;
		cpMap GetMap(std::string name) const;


		cpTex NO_TEXTURE;
	private:
		//unsigned int mNumUnloadedAssets;

		std::vector<Texture> mTextures;
		std::vector<AnimationSet> mAnimationSets;
		std::vector<Map> mMaps;
	};
}
