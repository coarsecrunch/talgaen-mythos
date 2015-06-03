#pragma once

#include "AAsset.h"
#include "Point.h"
#include "Cmn.h"
#include "IRenderable.h"
#include "Rectangle.h"
#include "Math/Vector2.h"

#include <vector>

namespace talga
{
	class Renderer;
	typedef std::array<vec2, 4> UVFrame;
	typedef std::pair<cpTex, UVFrame> Tile;

	class Map : public AAsset, public IRenderable
	{
	public:
		Map();
		Map(const Map& cpy);
		virtual ~Map();

		virtual void render(Renderer* renderer, const Camera* camera) const override;

		virtual bool load(std::string path, AssetManager& manager) override;
		virtual void destroy() override;

		const Map& operator=(const Map& cpy);
		const Tile& operator()(I32 column, I32 row) const;

		const Tile* TileAt(I32 x, I32 y) const;
        iPnt CartToTile(I32 x, I32 y) const;
        iPnt TileToCart(I32 x, I32 y) const;
		bool Exists(I32 x, I32 y) const; //nullptr
		I32 getTileIndex(I32 x, I32 y) const;
		I32 getTileIndex(const Tile& t) const;

		I32 size() const { return mWidth * mHeight; }
		I32 tileWidth() const { return mTileWidth; }
		I32 tileHeight() const { return mTileHeight; }

		I32 getWidth() const { return mWidth; }
		I32 getHeight() const { return mHeight; }
		I32 getTileHeight() const { return mTileHeight; }
		I32 getTileWidth() const { return mTileWidth; }
		
	protected:
		I32 mTileWidth;
		I32 mTileHeight;

		I32 mHeight;
		I32 mWidth;

		std::vector<Tile> mTileSet;
		std::vector<I32> mMap;
	};

}
