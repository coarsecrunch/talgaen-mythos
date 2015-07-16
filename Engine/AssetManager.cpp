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



namespace talga
{
	/* Amounts for the max amounts for stored assets. If vector are constantly dynamically allocated
	then the addresses for the elements change and the pointers to them become invalid */
	const I32 MAX_ANIMATIONS = 100;
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
	}
}
