#include "AssetManager.h"
#include <GL/glew.h>
#include "Rendering.h"
#include "Camera.h"
#include "Texture.h"
#include "Rect.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Cmn.h"
#include "AnimSet.h"
#include "font.h"
#include "luareg.h"
#include "Rect.h"

namespace talga
{
	/* Amounts for the max amounts for stored assets. If vector are constantly dynamically allocated
	then the addresses for the elements change and the pointers to them become invalid */
	const I32 MAX_ANIMATIONS = 1000;
	const I32 MAX_TEXTURES = 1000;
	const I32 MAX_MAPS = 50;
	cpTex AssetManager::NO_TEXTURE = nullptr;

	AssetManager::AssetManager()
		: mTextures{}
		, mMaps{}
		, mAnimationSets{}
	{
		mAnimationSets.reserve(MAX_ANIMATIONS);
		mTextures.reserve(MAX_TEXTURES);
		mMaps.reserve(MAX_MAPS);

		//AddTexture("../assets/notex.png");
		//NO_TEXTURE = GetTexture("notex.png");
	}

	cpTex AssetManager::AddTexture(std::string path)
	{
		Texture tex;

		if (tex.load(path, *this))
		{
			TALGA_MSG(path + " was successfully loaded");
			
			cpTex exists = static_cast<cpTex>(assetExists(tex.getName()));
			
			TALGA_WARN(!exists, path + " texture has already been added");
			if (exists)
				return exists;

			mTextures.push_back(tex);
			return &mTextures.back();
		}
		else
		{
			TALGA_WARN(0, path + " could not be loaded");
			tex.destroy();
			return nullptr;
		}
		
	}



	cpMap AssetManager::AddMap(std::string path)
	{
		Map map;

		if (map.load(path, *this))
		{
			cpMap exists = static_cast<cpMap>(assetExists(map.getName()));
			if (exists)
				return exists;

			TALGA_MSG(path + " was successfully loaded");
			mMaps.push_back(map);
			return &mMaps.back();
		}
		else
		{
			TALGA_WARN(0, path + " could not be loaded");
		}
	}

	cpAnimSet AssetManager::AddAnimationSet(std::string name, std::string texName, OOLUA::Lua_table_ref tbl)
	{
		if (tbl.valid())
		{
			cpTex tex = GetTexture(texName);
			
			if (!tex)
			{
				TALGA_WARN(0, "failed to add animation set " + name + " because texture " + texName + " could not be found");
				return nullptr;
			}
			AnimationSet set(GetTexture(texName), name);
			OOLUA::Table tableTable(tbl);

			oolua_pairs(tableTable)
			{
				std::vector<Rect> rectFrames;
				OOLUA::Lua_table_ref animation;
				std::string keyTop;
				OOLUA::pull(tableTable.state(), animation);
				OOLUA::pull(tableTable.state(), keyTop);
				OOLUA::Table iterAnimation(animation);
				TALGA_ASSERT(animation.valid(), "table invalid");

				oolua_pairs(iterAnimation)
				{
					OOLUA::Lua_table_ref frame;
					Rect rectFrame;
					OOLUA::pull(tableTable.state(), frame);
					TALGA_ASSERT(frame.valid(), "table invalid");

					OOLUA::Table frameTbl(frame);

					frameTbl.at(1, rectFrame.x);
					frameTbl.at(2, rectFrame.y);
					frameTbl.at(3, rectFrame.w);
					frameTbl.at(4, rectFrame.h);
					rectFrames.push_back(rectFrame);
				
					
				}
				oolua_pairs_end()


				set.addAnim(keyTop, rectFrames);
				OOLUA::push(tableTable.state(), keyTop);
			}
			oolua_pairs_end()

			mAnimationSets.push_back(set);
			TALGA_MSG("successfully created animation set " + name)
			return &mAnimationSets.back();
		}

		TALGA_WARN(0, "failed to add animation " + name);
		return nullptr;
	}

  cpFont AssetManager::addFont(const std::string& path, I32 size)
  {
	  Font font(size);

	  if (font.load(path, *this))
	  {
		  cpFont exists = static_cast<cpFont>(assetExists(font.getName()));
		  if (exists)
			  return exists;

		  TALGA_MSG(path + " was successfully loaded");
		  mFonts.push_back(font);
		  return &mFonts.back();
	  }
	  else
	  {
		  TALGA_WARN(0, path + " could not be loaded");
	  }
  }

  cpFont AssetManager::getFont(const std::string& name) const
  {
	  for (int i = 0; i < mFonts.size(); ++i)
	  {
		  if (mFonts[i].getName() == name)
			  return &mFonts[i];
	  }

	  TALGA_WARN(0, std::string("failed to find font ") + name);
	  return nullptr;
  }

	static vec3 UV(float x, float xMax, float y, float yMax, float frameH)
	{
		return vec3(x / xMax, (y / yMax));
	}

	cpMap AssetManager::GetMap(std::string name) const
	{
		for (int i = 0; i < mMaps.size(); ++i)
		{
			if (mMaps[i].getName() == name)
				return &mMaps[i];
		}

		TALGA_WARN(0, std::string("failed to find map ") + name);
		return nullptr;
	}

	cpTex AssetManager::GetTexture(std::string name) const
	{
		for (int i = 0; i < mTextures.size(); ++i)
		{
			if (mTextures[i].getName() == name)
			{
				return &mTextures[i];
			}
		}

		TALGA_WARN(0, std::string("failed to find texture ") + name);
		return nullptr;
	}

	const AAsset* AssetManager::assetExists(const std::string& name) const
	{
		for (auto it = mTextures.begin(); it != mTextures.end(); ++it)
		{
			if (it->getName() == name)
			{
				return &(*it);
			}
		}

		for (auto it = mMaps.begin(); it != mMaps.end(); ++it)
		{
			if (it->getName() == name)
			{
				return &(*it);
			}
		}

		return nullptr;
	}

	AssetManager::~AssetManager()
	{
		for (Texture& tex : mTextures)
		{
			tex.destroy();
		}

		for (Map& map : mMaps)
		{
			map.destroy();
		}

		for (Font& font : mFonts)
		{
			font.destroy();
		}
	}
}
