#include "Map.h"
#include "Cmn.h"
#include "Texture.h"
#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include <algorithm>
#include <fstream>
#include "Math/RandomGen.h"
#include "Math/Operations.h"

#define RELATIVE_ASSETS_PATH std::string("../assets/")

namespace talga
{
	Map::Map()
		: AAsset()
		, mWidth{ -1 }
		, mHeight{-1}
		, mTileSet{}
		, mTileWidth{-1}
		, mTileHeight{ -1 }
		, mMap{}
	{
	}

	Map::Map(const Map& cpy)
		: AAsset(cpy)
		, mTileSet(cpy.mTileSet)
		, mWidth(cpy.mWidth)
		, mHeight(cpy.mHeight)
		, mMap(cpy.mMap)
		, mTileHeight(cpy.mTileHeight)
		, mTileWidth(cpy.mTileWidth)
	{
	}

	const Map& Map::operator=(const Map& cpy)
	{
		AAsset::operator=(cpy);
		mTileSet = cpy.mTileSet;
		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mTileHeight = cpy.mTileHeight;
		mTileWidth = cpy.mTileHeight;
    mMap = cpy.mMap;

		return *this;
	}


	void Map::render(Renderer* renderer, const Camera* camera) const
	{
		I32 tileY = CartToTile(camera->getBox().getX() - (camera->getBox().getW() * 0.5f), 
			camera->getBox().getY() - (camera->getBox().getH() * 0.5f)).y;
		I32 tileX = CartToTile(camera->getBox().getX() - (camera->getBox().getW() * 0.5f),
			camera->getBox().getY() - (camera->getBox().getH() * 0.5f)).x;

		Rectangle tempR((I32)mTileWidth, (I32)mTileHeight);

		tempR.setZ(0.1f);

		for (I32 y = tileY; y <= camera->getBox().getH() / mTileHeight + tileY + 1; ++y)
		{
			for (I32 x = tileX; x <= camera->getBox().getW() / mTileWidth + tileX + 1; ++x)
			{
				if (!Exists(x, y))
					continue;

				if (getTileIndex(x, y) == 0)
					continue;
				
				tempR.setX( I32(x * mTileWidth + (0.5f * mTileWidth)) );
				tempR.setY( I32(y * mTileHeight + (0.5f * mTileHeight)) );
				tempR.updateVertsPosition();

				renderer->submit(tempR, TileAt(x, y)->first, 1.0f, TileAt(x, y)->second);
			}
		}


	}

	const Tile* Map::TileAt(I32 x, I32 y) const
	{
		if (!Exists(x, y))
			return nullptr;

		return &mTileSet[mMap[y * mWidth + x] - 1];
	}

	bool Map::Exists(I32 x, I32 y) const
	{
		return !(x >= mWidth || y >= mHeight || x < 0 || y < 0 );
	}

	Point Map::CartToTile(I32 x, I32 y) const
	{
		return Point{ x / mTileWidth, y / mTileHeight };
	}

	Point Map::TileToCart(I32 x, I32 y) const
	{
		return Point{ x * mTileWidth, y * mTileHeight };
	}

	I32 Map::getTileIndex(I32 x, I32 y) const
	{
		return mMap[y * mWidth + x];
	}


	bool Map::load(std::string path, AssetManager& manager)
	{
		std::ifstream stream;

		stream.open(path);
		
		if (!stream.is_open())
			return false;

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
			cpTex tex = manager.GetTexture(tempTex);

			if (!tex)
			{
				manager.AddTexture(RELATIVE_ASSETS_PATH + tempTex);
				tex = manager.GetTexture(tempTex);
			}


			if (tex)
			{
				I32 framesPerRow = tex->w() / tileWidth;
				I32 framesPerColumn = tex->h() / tileHeight;

				for (auto y = 0; y < framesPerColumn; ++y)
				{
					for (auto x = 0; x < framesPerRow; ++x)
					{
						//TL TR BR BL

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
				tiles.push_back(Tile{ tex, UVFrame{ { vec2{ 0.0f, 1.0f }, vec2{ 1.0f, 1.0f }, vec2{ 1.0f, 0.0f }, vec2{ 0.0f, 0.0f } } } });
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

		mName = name;
		mTileSet = tiles;
		mMap = mapdata;
		mWidth = mapWidth;
		mHeight = mapHeight;
		mTileWidth = tileWidth;
		mTileHeight = tileHeight;

		return true;
	}

	void Map::destroy()
	{
	}

	Map::~Map()
	{
	}
}
