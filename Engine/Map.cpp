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
		, mRects{}
	{
		updateRects();
	}

	Map::Map(const Map& cpy)
		: AAsset(cpy)
		, mTileSet(cpy.mTileSet)
		, mWidth(cpy.mWidth)
		, mHeight(cpy.mHeight)
		, mMap(cpy.mMap)
		, mTileHeight(cpy.mTileHeight)
		, mTileWidth(cpy.mTileWidth)
		, mRects(cpy.mRects)
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
		mRects = cpy.mRects;


		return *this;
	}

	void Map::render(Renderer* renderer, const Camera* camera) const
	{
		I32 tileY = CartToTile(camera->getBox().getX(), camera->getBox().getY()).y;
		I32 tileX = CartToTile(camera->getBox().getX(), camera->getBox().getY()).x;

		for (I32 y = tileY; y <= camera->getBox().getH() / mTileHeight + tileY; ++y)
		{
			for (I32 x = tileX; x <= camera->getBox().getW() / mTileWidth + tileX; ++x)
			{
				if (!Exists(x, y))
					continue;
				
				renderer->submit(mRects[y * mWidth + x], TileAt(x, y)->first, TileAt(x,y)->second);
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
					vec3{ x * mTileWidth + (mTileWidth * 0.5f), y * mTileHeight + (mTileHeight * 0.5f) },
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


	void Map::load(std::string path, AssetManager& manager)
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
			cpTex tex = manager.GetTexture(tempTex);

			if (!tex)
			{
				manager.AddTexture(RELATIVE_ASSETS_PATH + tempTex);
				tex = manager.GetTexture(tempTex);
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

		updateRects();
	}

	void Map::destroy()
	{
	}

	Map::~Map()
	{
	}
}