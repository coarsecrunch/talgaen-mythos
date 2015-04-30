#include "Map.h"
#include "Cmn.h"
#include "Texture.h"
#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include <algorithm>
#include "Math/RandomGen.h"

namespace talga
{
	Map::Map(std::string mapName, std::vector<Tile> tileSet, cpAnim sheet, const std::vector<I32>& map, I32 width, I32 height, I32 tileWidth, I32 tileHeight)
		: mName(mapName)
		, mWidth(width)
		, mHeight(height)
		, mTileSet(tileSet)
		, mTileWidth(tileWidth)
		, mTileHeight(tileHeight)
		, mSheet(sheet)
		, mMap(map)
		, mRects()
	{
		updateRects();
	}

	Map::Map(const Map& cpy)
		: mName(cpy.mName)
		, mTileSet(cpy.mTileSet)
		, mSheet(cpy.mSheet)
		, mWidth(cpy.mWidth)
		, mHeight(cpy.mHeight)
		, mMap(cpy.mMap)
		, mTileHeight(cpy.mTileHeight)
		, mTileWidth(cpy.mTileWidth)
		, mNumTiles(cpy.mNumTiles)
		, mRects(cpy.mRects)
	{
	}

	const Map& Map::operator=(const Map& cpy)
	{
		mName = cpy.mName;
		mTileSet = cpy.mTileSet;
		mSheet = cpy.mSheet;
		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mTileHeight = cpy.mTileHeight;
		mTileWidth = cpy.mTileHeight;
		mNumTiles = cpy.mNumTiles;
		mRects = cpy.mRects;


		return *this;
	}

	void Map::render(Renderer* renderer, const Camera* camera) const
	{
		I32 tileY = CartToTile(camera->getX(), camera->getY()).y;
		I32 tileX = CartToTile(camera->getX(), camera->getY()).x;

		for (I32 y = tileY; y <= camera->getH() / mTileHeight + tileY; ++y)
		{
			for (I32 x = tileX; x <= camera->getW() / mTileWidth + tileX; ++x)
			{
				if (!Exists(x, y))
					continue;
				
				renderer->submit(mRects[y * mWidth + x]);
			}
		}
	}

	void Map::updateRects()
	{
		mRects.clear();

		for (I32 y = 0; y < mHeight; ++y)
		{
			for (I32 x = 0; x < mWidth; ++x)
			{

				mRects.push_back(Rectangle(mTileWidth, mTileHeight,
					vec3{ x * mTileWidth + (mTileWidth * 0.5f), y * mTileHeight + (mTileHeight *0.5f) },
					vec4{ get01f(), get01f(), get01f() }
				));

				
				mRects.back().updateVertsPosition();
			}
			
		}

	}
	


	const Tile* Map::TileAt(I32 x, I32 y) const
	{
		if (!Exists(x, y))
			return nullptr;

		return &mTileSet[mMap[y*mHeight + x]];
	}

	bool Map::Solid(I32 x, I32 y) const
	{
		return mTileSet[mMap[y*mHeight + x]].solid;
	}

	bool Map::Exists(I32 x, I32 y) const
	{
		return !(x >= mWidth || y >= mHeight || x < 0 || y < 0);
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

	Map::~Map()
	{
	}
}