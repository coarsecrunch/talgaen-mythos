#include "Map.h"
#include "Cmn.h"
#include "Texture.h"
#include "Camera.h"
#include "Game.h"
#include <cstring>
#include <cassert>

namespace talga
{
	Map::Map(Game* game, std::string mapName, std::vector<Tile> tileSet, cpAnim sheet, const I32* map, I32 width, I32 height, I32 tileWidth, I32 tileHeight)
		: ARenderableObj(game, std::vector<cpAnim>(), vec4(0.0f, 0.0f, 0.0f, 1.0f), width, height)
		, mName(mapName)
		, mTileSet(tileSet)
		, mTileWidth(tileWidth)
		, mTileHeight(tileHeight)
		, mSheet(sheet)
		, mMap(new I32[width * height])
	{
		memcpy(mMap, map, sizeof(I32) * width * height);
	}

	Map::Map()
	{

	}

	Map::Map(const Map& cpy)
		: ARenderableObj(cpy)
		, mName(cpy.mName)
		, mTileSet(cpy.mTileSet)
		, mSheet(cpy.mSheet)
		, mMap(new I32[mWidth * mWidth])
		, mTileHeight(cpy.mTileHeight)
		, mTileWidth(cpy.mTileWidth)
		, mNumTiles(cpy.mNumTiles)
	{
		memcpy(mMap, cpy.mMap, sizeof(I32) * mWidth * mWidth);
	}

	const Map& Map::operator=(const Map& cpy)
	{
		ARenderableObj::operator=(cpy);

		mName = cpy.mName;
		mTileSet = cpy.mTileSet;
		mSheet = cpy.mSheet;
		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mTileHeight = cpy.mTileHeight;
		mTileWidth = cpy.mTileHeight;
		mNumTiles = cpy.mNumTiles;
		mMap = new I32[mWidth * mHeight];
		memcpy(mMap, cpy.mMap, sizeof(I32) * mWidth * mHeight);

		return *this;
	}

	void Map::Render(const Camera& camera)
	{
		glBindVertexArray(Camera::mVAOs[Camera::TEXTURED_QUAD]);
		glUseProgram(Camera::mProgram);
		glViewport(0, 0, camera.getW(), camera.getH());

		mSheet->tex->Bind();

		mat4 fix;

		I32 tileY = CartToTile(camera.getX(), camera.getY()).y;
		I32 tileX = CartToTile(camera.getX(), camera.getY()).x;

		for (I32 y = tileY; y <= camera.getH() / mTileHeight + tileY; ++y)
		{
			for (I32 x = tileX; x <= camera.getW() / mTileWidth + tileX; ++x)
			{
				if (!Exists(x, y))
					continue;

				U32 offset = mSheet->getOffset(getTileIndex(x, y));

				fix = mat4(1.0f, 0.0f, 0.0f, TileToCart(x, y).x + (0.5f * mTileWidth),
					0.0f, 1.0f, 0.0f, TileToCart(x, y).y + (mTileHeight * 0.5f),
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);


				fix = camera.getProjectionMatrix() * camera.getCameraMat()
					* mat4(1.0f, 0.0f, 0.0f, TileToCart(x, y).x + (0.5f * mTileWidth),
					0.0f, 1.0f, 0.0f, TileToCart(x, y).y + (mTileHeight * 0.5f),
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				glUniformMatrix4fv(Camera::MVPShaderLoc, 1, GL_TRUE, &fix[0][0]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void*)offset);
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glFlush();
	}

	void Map::Update(F32 dt)
	{

	}

	Tile* Map::TileAt(I32 x, I32 y)
	{
		if (!Exists(x, y))
			return nullptr;

		return &mTileSet[mMap[y*mHeight + x]];
	}

	bool Map::Solid(I32 x, I32 y)
	{
		assert(Exists(x, y));

		return mTileSet[mMap[y*mHeight + x]].solid;
	}

	bool Map::Exists(I32 x, I32 y)
	{
		return !(x >= mWidth || y >= mHeight || x < 0 || y < 0);
	}

	Point Map::CartToTile(I32 x, I32 y)
	{
		return Point{ x / mTileWidth, y / mTileHeight };
	}

	Point Map::TileToCart(I32 x, I32 y)
	{
		return Point{ x * mTileWidth, y * mTileHeight };
	}

	I32 Map::getTileIndex(I32 x, I32 y)
	{
		assert(Exists(x, y));

		return mMap[y * mWidth + x];
	}

	Map::~Map()
	{
		delete[] mMap;
	}
}