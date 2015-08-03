#pragma once

#include <string>
#include <vector>
#include <array>

#include "Math/Vector2.h"
#include "Cmn.h"
#include "oolua/oolua_table.h"

namespace talga
{
	class AnimationSet;
	class Texture;
	class Map;
	class Font;
	class AAsset;
	struct Rect;
	typedef std::array<vec2, 4> UVFrame;
	typedef std::vector<UVFrame> UVAnimation;
	typedef std::pair<std::string, UVAnimation> AnimSetPair;
	
	class AssetManager
	{
	public:
		static const U32 MAX_ASSETS = 10000;
		AssetManager();
		~AssetManager();

		cpTex AddTexture(std::string path);
		cpAnimSet AddAnimationSet(std::string name, std::string texName, std::vector<AnimSetPair>);
		cpAnimSet AddAnimationSet(std::string name, std::string texName, OOLUA::Lua_table_ref tbl);
		cpMap AddMap(std::string path);
		cpAsset AddAsset(std::string path);
		cpTex GetTexture(std::string name) const;
		cpMap GetMap(std::string name) const;
		
		cpAsset GetAsset(const std::string& name) const;

		cpScript AddScript(const std::string& path);
		cpScript GetScript(const std::string& name) const;


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
		std::vector<Script> mScripts;
	};
}
