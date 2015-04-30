#pragma once

#include <string>
#include <vector>
#include "Cmn.h"

namespace talga
{
	class Animation;
	class AnimationSet;
	class Texture;
	class Map;
	struct Rect;

	class AssetManager
	{
	public:
		enum ASSET_TYPE { ASSET_TEXTURE, ASSET_ANIMATION, ASSET_MAP };

		enum Buffer_IDs { ARRAY_BUFFER_TEXTURED_QUAD, ELEMENT_BUFFER_TEXTURED_QUAD, numBuffers };
		enum Attrib_IDs { TEXTURED_QUAD_vPosition = 0, TEXTURED_QUAD_vUvCoord = 1 };

		static const unsigned int MAX_ASSETS = 10000;

		AssetManager();
		~AssetManager();

		void AddAnimation(std::string texName, std::string name, std::vector<Rect> frames);
		void LoadAnimations();//GLDep
		void AddTexture(std::string path);
		void AddAnimationSet(std::string name, const AnimationSet& anim);
		void LoadMap(std::string path);
		cpAnim GetAnimation(std::string name) const;
		cpTex GetTexture(std::string name) const;
		cpMap GetMap(std::string name) const;

	private:
		struct toLoadAnim
		{
			std::string name;
			std::vector < Rect > frames;
			cpTex tex;
		};

		U32 mBuffers[numBuffers];
		//unsigned int mNumUnloadedAssets;
		U32 mNumTextures;

		std::vector<toLoadAnim> mAnimationsToLoad;
		std::vector<Texture> mTextures;
		std::vector<Animation> mAnimations;
		std::vector<AnimationSet> mAnimationSets;
		std::vector<Map> mMaps;
	};
}
