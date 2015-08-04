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
#include "sys.h"
#include "Script.h"

#define RELATIVE_ASSETS_PATH std::string("../assets/")

namespace talga
{
const I32 MAP_MAX_COLLIDERS = 1000;
#ifdef TALGA_QT_BUILD
  const int VERT_SIZE = 6;
#endif
	Map::Map()
		: AAsset()
		, mWidth{ -1 }
		, mHeight{ -1 }
		, mTileSet{}
		, mTileWidth{ -1 }
		, mTileHeight{ -1 }
		, mLayers{}
		, mTileSheets{}
		, mIsSaved{false}
		, mStaticSceneGeom()
		, mRenderSceneGeom(false)
    , mInitScript(nullptr)
	{
		mLayers.reserve(MAX_LAYERS);
		mStaticSceneGeom.reserve(MAP_MAX_COLLIDERS);
	}

	Map::Map(const Map& cpy)
		: AAsset(cpy)
		, mTileSet(cpy.mTileSet)
		, mWidth(cpy.mWidth)
		, mHeight(cpy.mHeight)
		, mLayers(cpy.mLayers)
		, mTileHeight(cpy.mTileHeight)
		, mTileWidth(cpy.mTileWidth)
		, mTileSheets(cpy.mTileSheets)
		, mIsSaved{cpy.mIsSaved}
		, mStaticSceneGeom(cpy.mStaticSceneGeom)
		, mRenderSceneGeom(cpy.mRenderSceneGeom)
		, mInitScript(cpy.mInitScript)
	{
	}

	MapLayer::MapLayer(I32 size, std::string layerName)
		: mIndices(size)
		, mVisible{ true }
		, mSolid{ false }
		, mName( layerName )
	{
	}

	MapLayer::MapLayer(const MapLayer& cpy)
		: mIndices(cpy.mIndices)
		, mVisible(cpy.mVisible)
		, mSolid(cpy.mSolid)
		, mName( cpy.mName )
	{
	}

	const MapLayer& MapLayer::operator=(const MapLayer& cpy)
	{
		mIndices = cpy.mIndices;
		mVisible = cpy.mVisible;
		mSolid = cpy.mSolid;
		mName = cpy.mName;

		return *this;
	}

	const Map& Map::operator=(const Map& cpy)
	{
		AAsset::operator=(cpy);
		mTileSet = cpy.mTileSet;
		mWidth = cpy.mWidth;
		mHeight = cpy.mHeight;
		mTileHeight = cpy.mTileHeight;
		mTileWidth = cpy.mTileHeight;
		mLayers = cpy.mLayers;
		mTileSheets = cpy.mTileSheets;
		mIsSaved = cpy.mIsSaved;
		mStaticSceneGeom = cpy.mStaticSceneGeom;
		mRenderSceneGeom = cpy.mRenderSceneGeom;
		mInitScript = cpy.mInitScript;

		return *this;
	}

	void Map::render(Renderer* renderer, const Camera* camera) const
	{
		I32 tileY = CartToTile(camera->box().getX() - (camera->box().getW() * 0.5f), 
      camera->box().getY() - (camera->box().getH() * 0.5f)).y();
		I32 tileX = CartToTile(camera->box().getX() - (camera->box().getW() * 0.5f),
      camera->box().getY() - (camera->box().getH() * 0.5f)).x();

		Rectangle tempR((I32)mTileWidth, (I32)mTileHeight);

		tempR.setZ(0.1f);

		for (I32 y = tileY; y <= camera->box().getH() / mTileHeight + tileY + 1; ++y)
		{
			for (I32 x = tileX; x <= camera->box().getW() / mTileWidth + tileX + 1; ++x)
			{
				if (!Exists(x, y))
					continue;
				tempR.setZ(0.1f);

				for (I32 i = 0; i < mLayers.size(); ++i)
				{
					//could me much more optimal
					if (!mLayers[i].isVisible()) 
						continue;
					if (getTileIndex(x, y, i) == 0)
						continue;

					tempR.setX(I32(x * mTileWidth + (0.5f * mTileWidth)));
					tempR.setY(I32(y * mTileHeight + (0.5f * mTileHeight)));
					tempR.updateVertsPosition();

					renderer->submit(tempR, getTile(x, y, i).first, 1.0f, getTile(x, y, i).second);
					tempR.setZ(tempR.getZ() + 0.01f);
				}
				
			}
		}

	if (mRenderSceneGeom)
    {
      for (auto it = mStaticSceneGeom.cbegin(); it != mStaticSceneGeom.cend(); ++it)
        (*it)->render(renderer, camera);
    }

	}

	const Tile& Map::getTile(I32 x, I32 y, I32 layerIndex) const
	{
		TALGA_ASSERT(Exists(x, y), "tried to access non existent tile");
		I32 idx = mLayers[layerIndex][y * mWidth + x] - 1;
		//if (idx <= 0 || idx >= mTileSet.size()) return BLANK_TILE;
		return mTileSet[idx];
	}

  Tile Map::getTile(I32 x, I32 y, I32 layerIndex)
  {
    //if (!Exists(x, y))
      //return BLANK_TILE;
    TALGA_ASSERT(Exists(x, y), "tried to access non existent tile");
    I32 idx = mLayers[layerIndex][y * mWidth + x] - 1;
    if (idx >= 0)
      return mTileSet[idx];
    else
      return BLANK_TILE;
  }

	bool Map::Exists(I32 x, I32 y) const
	{
		return !(x >= mWidth || y >= mHeight || x < 0 || y < 0 );
	}

    iPnt Map::CartToTile(I32 x, I32 y) const
	{
        return iPnt{x / mTileWidth, y / mTileHeight };
	}

    iPnt Map::TileToCart(I32 x, I32 y) const
	{
        return iPnt{ x * mTileWidth, y * mTileHeight };
	}
	
	I32 Map::getTileIndex(I32 x, I32 y, I32 layerIndex) const
	{
		TALGA_ASSERT(Exists(x, y), "tried to get index of non existent tile");
		return mLayers[layerIndex][y * mWidth + x];
	}
	bool Map::load(std::string path, AssetManager& manager)
	{
    *this = createEmptyMap(-1,-1,-1,-1, "", "", manager);
		std::ifstream stream;

		stream.open(path);
		
		if (!stream.is_open())
			return false;

		std::string tempName = getFileNameFromPath(path);
		std::string tempPath;
		if (!isAbs(path))
			tempPath = getAbsFromRel(getWorkingDirectory(), getPathFromFilePath(path));
		else
			tempPath = getPathFromFilePath(path);

		std::string initScriptPath;
		I32 tileWidth;
		I32 tileHeight;
		I32 mapWidth;
		I32 mapHeight;
		I32 numTextures;
		I32 numLayers;
		I32 numRects;
		I32 numTris;

		std::vector<Tile> tiles;
		std::string tempTexPath;

		stream >> initScriptPath;
		stream >> tileWidth;
		stream >> tileHeight; // tileHeight
		stream >> mapWidth; // mapWidth
		stream >> mapHeight; // mapHeight
		stream >> numRects;
		stream >> numTris;
		stream >> numTextures; // numTiles
		stream >> numLayers;
		

    if (initScriptPath != "null")
    {
      mInitScript = manager.AddScript(getAbsFromRel(tempPath, getPathFromFilePath(initScriptPath)) + getFileNameFromPath(initScriptPath));

#ifndef TALGA_QT_BUILD
      mInitScript->execute();


      OOLUA::Lua_func_ref initFunc = LuaEngine::instance()->getGlobalFunction("init");
		
      if (!initFunc.valid())
      {
        TALGA_WARN(0, "failed to find lua init function for map " + tempName);
        return false;
      }

      OOLUA::Lua_function tempCall(initFunc.state());

      if (!tempCall(initFunc))
        LuaEngine::instance()->reportError();
#endif
    }
		for (auto i = 0; i < numTextures; ++i)
		{
			stream >> tempTexPath;

			std::string tName = getAbsFromRel(tempPath, getPathFromFilePath(tempTexPath)) + getFileNameFromPath(tempTexPath);
			cpTex tex = manager.AddTexture(tName);

			if (tex)
			{
				mTileSheets.push_back(tex);
				I32 framesPerRow = tex->w() / tileWidth;
				I32 framesPerColumn = tex->h() / tileHeight;

				for (auto y = 0; y < framesPerColumn; ++y)
				{
					for (auto x = 0; x < framesPerRow; ++x)
					{
						//TL TR BR BL
            //REAL BL BR TR TL
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
        return false;
				tiles.push_back(Tile{ tex, UVFrame{ { vec2{ 0.0f, 1.0f }, vec2{ 1.0f, 1.0f }, vec2{ 1.0f, 0.0f }, vec2{ 0.0f, 0.0f } } } });
			}
		}

    for (int i = 0; i < numRects; ++i)
    {
      F32 x = -1;
      F32 y = -1;
      F32 w = -1;
      F32 h = -1;

      stream >> x;
      stream >> y;
      stream >> w;
      stream >> h;

      RdrRect* tmpRect = new RdrRect(w, h, x, y, vec4(0.0f, 0.0f, 1.0f, 1.0f));
#ifdef TALGA_QT_BUILD
      RdrRect* TL = new RdrRect(VERT_SIZE, VERT_SIZE, tmpRect->box().getW() * -0.5f, tmpRect->box().getH() * -0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
      RdrRect* TR = new RdrRect(VERT_SIZE, VERT_SIZE, tmpRect->box().getW() * 0.5f, tmpRect->box().getH() * -0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
      RdrRect* BR = new RdrRect(VERT_SIZE, VERT_SIZE, tmpRect->box().getW() * 0.5f, tmpRect->box().getH() * 0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
      RdrRect* BL = new RdrRect(VERT_SIZE, VERT_SIZE, tmpRect->box().getW() * -0.5f, tmpRect->box().getH() * 0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));

      tmpRect->addChild(TL);
      tmpRect->addChild(TR);
      tmpRect->addChild(BR);
      tmpRect->addChild(BL);
#endif
      mStaticSceneGeom.push_back(tmpRect);
    }

    for (int i = 0; i < numTris; ++i)
    {
      F32 x;
      F32 y;
      vec2 T;
      vec2 BR;
      vec2 BL;

      stream >> x;
      stream >> y;
      stream >> T[0];
      stream >> T[1];
      stream >> BR[0];
      stream >> BR[1];
      stream >> BL[0];
      stream >> BL[1];

      RdrTri* tmpTri = new RdrTri( {T, BR, BL}, x, y, vec4(0.0f, 1.0f, 0.0f, 1.0f) );
#ifdef TALGA_QT_BUILD
      RdrRect* TT = new RdrRect(VERT_SIZE, VERT_SIZE, T[0], T[1], vec4(1.0f, 0.0f, 1.0f, 1.0f));
      RdrRect* BRR = new RdrRect(VERT_SIZE, VERT_SIZE,BR[0], BR[1], vec4(1.0f, 0.0f, 1.0f, 1.0f));
      RdrRect* BLL = new RdrRect(VERT_SIZE, VERT_SIZE, BL[0],BL[1], vec4(1.0f, 0.0f, 1.0f, 1.0f));

      tmpTri->addChild(TT);
      tmpTri->addChild(BRR);
      tmpTri->addChild(BLL);
#endif
      mStaticSceneGeom.push_back(tmpTri);
    }

		std::vector<std::string> layerNames(numLayers);

		for (I32 i = 0; i < numLayers; ++i)
		{
			stream >> layerNames[i];
		}

		std::vector<MapLayer> layers(numLayers);

		I32 tempCount = 0;

		for (auto layerIter = layers.begin(); layerIter != layers.end(); ++layerIter)
		{
			(*layerIter) = MapLayer(mapWidth * mapHeight, layerNames[tempCount]);

			for (int y = 0; y < mapHeight; ++y)
			{
				for (int x = 0; x < mapWidth; ++x)
				{
					stream >> (*layerIter)[y * mapWidth + x];
				}
			}

			++tempCount;
		}
		
		mNumSheets = numTextures;
		mName = tempName;
		mPath = tempPath;
		mTileSet = tiles;
		mLayers = layers;
		mWidth = mapWidth;
		mHeight = mapHeight;
		mTileWidth = tileWidth;
		mTileHeight = tileHeight;


		stream.close();
		return true;
	}
	bool Map::save(std::string path, AssetManager& manager)
	{
		std::ofstream stream;

		stream.open(path);

		if (!stream.is_open()) return false;

		mName = getFileNameFromPath(path);
		if (!isAbs(path))
			mPath = getAbsFromRel(getWorkingDirectory(), getPathFromFilePath(path));
		else
			mPath = getPathFromFilePath(path);
		
    if (mInitScript)
      stream << getRelFromAbs(getPath(), mInitScript->getPath())  + mInitScript->getName()<< std::endl;
    else
      stream << "null" << std::endl;
    stream << mTileWidth << std::endl;
		stream << mTileHeight << std::endl;
		stream << mWidth << std::endl;
		stream << mHeight << std::endl;

    std::vector<RdrRect*> rects;
    std::vector<RdrTri*> tris;

    for (auto it = mStaticSceneGeom.begin(); it != mStaticSceneGeom.end(); ++it)
    {
      if (dynamic_cast<RdrRect*>(*it))
      {
        rects.push_back((RdrRect*)*it);
      }
      else if (dynamic_cast<RdrTri*>(*it))
      {
        tris.push_back((RdrTri*)*it);
      }
    }

    stream << rects.size() << std::endl;
    stream << tris.size() << std::endl;

		std::string lastTex = "";
		std::vector<std::string> sheetNames;
    std::vector<std::string> sheetPaths;
		I32 numSheets = 0;
		for (auto it = mTileSet.begin(); it != mTileSet.end(); ++it)
		{
			if (it->first->getName() != lastTex)
			{
				lastTex = it->first->getName();
				++numSheets;
        std::string s = getRelFromAbs(getPath(), it->first->getPath()) + lastTex;
        sheetNames.push_back(s );
			}
		}

		stream << numSheets << std::endl;
		stream << mLayers.size() << std::endl;

    for (auto it = sheetNames.begin(); it != sheetNames.end(); ++it)
		{
      stream << *it << std::endl;
		}

    for (auto it = rects.begin(); it != rects.end(); ++it)
    {
      stream << (*it)->box().getX() << " " << (*it)->box().getY() << " " << (*it)->box().getW() << " " << (*it)->box().getH() << std::endl;
    }

    for (auto it = tris.begin(); it!= tris.end(); ++it)
    {
      auto verts = (*it)->getBase().getRealVerts();
      stream << (*it)->getBase().getX() <<  " " << (*it)->getBase().getY() <<  " " << verts[0].x() << " " <<
                verts[0].y() << " " << verts[1].x() << " " << verts[1].y() << " " << verts[2].x() << " " << verts[2].y() << std::endl;
    }

		for (auto it = mLayers.begin(); it != mLayers.end(); ++it)
		{
			stream << it->getName() << std::endl;
		}

		for (auto it = mLayers.begin(); it != mLayers.end(); ++it)
		{
			for (I32 y = 0; y < mHeight; ++y)
			{
				for (I32 x = 0; x < mWidth; ++x)
				{
					stream << (*it)[y * mWidth + x] << " ";
				}

				stream << std::endl;
			}
			stream << std::endl;
		}

		stream.close();

    mIsSaved = true;
		TALGA_MSG(path + " was sucessfully saved");
		return true;
	}

  Map Map::createEmptyMap(I32 tW, I32 tH, I32 w, I32 h, const std::string& name, const std::string& luaInitPath, AssetManager& manager)
  {
    Map map;

    map.mTileWidth = tW;
    map.mTileHeight = tH;
    map.mWidth = w;
    map.mHeight = h;
    map.mName = name;
    map.mPath = "";
    map.mLayers.push_back(MapLayer(w * h, "layer0"));
    map.mIsSaved = false;
    map.mInitScript = manager.AddScript(luaInitPath);

    return map;
  }

	void Map::destroy()
	{
	}

	Map::~Map()
	{
	}
}
