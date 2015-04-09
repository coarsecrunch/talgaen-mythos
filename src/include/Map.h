#pragma once

#include "Point.h"
#include "Cmn.h"
#include "ARenderableObj.h"
#include <vector>

namespace talga
{
	struct Tile
	{
		I32 frame;
		bool solid;
	};

	class Map : public ARenderableObj
	{
	public:
		Map(Game* game, std::string mapName, std::vector<Tile> tileSet, cpAnim sheet, const I32* map, I32 width, I32 height, I32 tileWidth, I32 tileHeight);
		Map();
		Map(const Map& cpy);
		~Map();

		virtual void Update(F32 dt) override;
		virtual void Render(const Camera& camera) override;

		const Map& operator=(const Map& cpy);

		Tile* TileAt(I32 x, I32 y);
		//const Tile& CartToTile(int x, int y);
		Point CartToTile(I32 x, I32 y);
		Point TileToCart(I32 x, I32 y);
		bool Exists(I32 x, I32 y); //nullptr
		bool Solid(I32 x, I32 y);
		I32 getTileIndex(I32 x, I32 y);

		I32 size() const { return mWidth * mHeight; }
		I32 tileWidth() const { return mTileWidth; }
		I32 tileHeight() const { return mTileHeight; }

		I32 getWidth() const { return mWidth; }
		I32 getHeight() const { return mHeight; }
		I32 getTileHeight() const { return mTileHeight; }
		I32 getTileWidth() const { return mTileWidth; }
		cpAnim getSheet() const { return mSheet; }
		std::string getName() const { return mName; }
	protected:
		I32 mTileWidth;
		I32 mTileHeight;
		I32 mNumTiles;
		cpAnim mSheet;
		std::vector<Tile> mTileSet;
		std::string mName;
		I32* mMap;
	};

}