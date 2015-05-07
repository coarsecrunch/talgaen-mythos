#include "AssetManager.h"
#include "GL\glew.h"
#include "Rendering.h"
#include "Camera.h"
#include "Texture.h"
#include "Rect.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Cmn.h"
#include "AnimSet.h"
#include "Math/Operations.h"

#define RELATIVE_ASSETS_PATH std::string("../assets/")

namespace talga
{
	/* Amounts for the max amounts for stored assets. If vector are constantly dynamically allocated
	then the addresses for the elements change and the pointers to them become invalid */
	const I32 MAX_ANIMATIONS = 100;
	const I32 MAX_TEXTURES = 1000;
	const I32 MAX_MAPS = 50;


	AssetManager::AssetManager()
		: mTextures{}
		, mMaps{}
		, mAnimationSets{}
		, NO_TEXTURE{ nullptr }
	{
		mAnimationSets.reserve(MAX_ANIMATIONS);
		mTextures.reserve(MAX_TEXTURES);
		mMaps.reserve(MAX_MAPS);

		//AddTexture(RELATIVE_ASSETS_PATH + "notex.png");
		//NO_TEXTURE = GetTexture("notex.png");
	}

	void AssetManager::AddTexture(std::string path)
	{
		mTextures.push_back(Texture{path});
	}

	void AssetManager::LoadMap(std::string path)
	{
		std::ifstream stream;

		stream.open(path);
		TALGA_ASSERT(stream.is_open(), std::string("failed to read map ") + path);

		char cc = '\0';
		I32 index = path.size();

		while (cc != '\\' && cc != '/')
		{
			--index;
			cc = path.at(index);
		}

		I32 startIndex = index + 1;
		I32 length = (path.size() - startIndex);
		std::string name = path.substr(startIndex, length);


		I32 tileWidth;
		I32 tileHeight;
		I32 mapWidth;
		I32 mapHeight;
		I32 numTextures;

		std::vector<Tile> tiles;
		std::string tempTex;

		stream >> tileWidth;
		stream >> tileHeight; // tileHeight
		stream >> mapWidth; // mapWidth
		stream >> mapHeight; // mapHeight
		stream >> numTextures; // numTiles

		for (auto i = 0; i < numTextures; ++i)
		{
			stream >> tempTex;
			cpTex tex = GetTexture(tempTex);
			
			if (!tex)
			{
				AddTexture( RELATIVE_ASSETS_PATH + tempTex);
				tex = GetTexture(tempTex);
			}
			
			TALGA_WARN(tex, std::string("map ") + name + " tried to load " + tempTex + " but could not");


			if (tex)
			{
				I32 framesPerRow = tex->w() / tileWidth;
				I32 framesPerColumn = tex->h() / tileHeight;

				for (auto y = 0; y < framesPerColumn; ++y)
				{
					for (auto x = 0; x < framesPerRow; ++x)
					{
						//TL TR BR BL
						/*tiles.push_back(Tile{ tex, UVFrame{ {
								GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y, tex->h()),
								GET_UV( (F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y, tex->h()),
								GET_UV((F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y + tileHeight, tex->h()),
								GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y + tileHeight, tex->h())
							} } });
						*/
						
						tiles.push_back(Tile{ tex, UVFrame{ {
								GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y + tileHeight, tex->h()),
						
						GET_UV((F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y + tileHeight, tex->h()),
						GET_UV((F32)tileWidth * x + tileWidth, tex->w(), (F32)tileHeight * y, tex->h()),
						GET_UV((F32)tileWidth * x, tex->w(), (F32)tileHeight * y, tex->h())
						} } });
						
					}
				}
			}
			else // push back default UVs for the NO_TEXTURE texture
			{
				tiles.push_back(Tile{ tex, UVFrame{{ vec2{0.0f, 1.0f}, vec2{1.0f, 1.0f}, vec2{1.0f, 0.0f}, vec2{0.0f, 0.0f} } }});
			}
		}

		std::vector<I32> mapdata(mapWidth * mapHeight);

		for (int y = 0; y < mapHeight; ++y)
		{
			for (int x = 0; x < mapWidth; ++x)
			{
				stream >> mapdata[y * mapWidth + x];
			}
		}

		Map map(name, tiles, mapdata, mapWidth, mapHeight, tileWidth, tileHeight);
		mMaps.push_back(map);
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
			if (mTextures[i].name == name)
			{
				return &mTextures[i];
			}
		}

		TALGA_WARN(0, std::string("failed to find texture ") + name);
		return nullptr;
	}
	AssetManager::~AssetManager()
	{
		for (Texture& tex : mTextures)
		{
			tex.destroy();
		}
	}
}