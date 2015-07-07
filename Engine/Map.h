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
  const Tile BLANK_TILE{nullptr, {vec2{0.0f, 0.0f},vec2{0.0f, 0.0f},vec2{0.0f, 0.0f},vec2{0.0f, 0.0f}}};
	class MapLayer
	{
	public:
		MapLayer() 
			: mIndices(), mVisible( true ), mSolid( false ), mName( "default" )
		{}

		MapLayer(I32 size, std::string layerName);
		MapLayer(const MapLayer& cpy);
		
		I32& operator[](I32 index) { return mIndices[index]; }
		I32 operator[](I32 index) const { return mIndices[index]; }

		const MapLayer& operator=(const MapLayer& cpy);

		bool isVisible() const { return mVisible; }
		bool isSolid() const { return mSolid; }
		std::string getName() const { return mName; }

		void setVisible(bool value) { mVisible = value; }
		void setSolid(bool value) { mSolid = value; }
		void resize(I32 size) { mIndices.clear();  mIndices.resize(size); }
		void setName(std::string value) { mName = value; }
	protected:
		std::vector<I32> mIndices;
		
		bool mVisible;
		bool mSolid;
		std::string mName;
	};

	typedef std::vector<MapLayer> LayerList;

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

        iPnt CartToTile(I32 x, I32 y) const;
        iPnt TileToCart(I32 x, I32 y) const;
		bool Exists(I32 x, I32 y) const; //nullptr

		Tile getTile(I32 x, I32 y, I32 layerIndex);
		const Tile& getTile(I32 x, I32 y, I32 layerIndex) const;
		I32 getTileIndex(const Tile& t) const;
		I32 getTileIndex(I32 x, I32 y, I32 layerIndex) const;

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
		LayerList mLayers;
	};

}
