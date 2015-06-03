#include "editormap.h"
#include "Rect.h"
#include "Math/Operations.h"
#include "Texture.h"

namespace talga
{
  namespace editor
  {

    EditorMap::EditorMap()
      : Map()
    {

    }

    EditorMap::~EditorMap()
    {

    }

    EditorMap::EditorMap(const EditorMap &cpy)
      : Map (cpy)
    {

    }

    const EditorMap &EditorMap::operator=(const EditorMap &cpy)
    {
      Map::operator =(cpy);
      return *this;
    }

    const EditorMap &EditorMap::operator=(const Map &cpy)
    {
      Map::operator =(cpy);
      return *this;
    }

    I32 EditorMap::getOffset(cpTex tex) const
    {
      for (I32 i = 0; i < mTileSet.size(); ++i)
      {
        if (tex == mTileSet[i].first)
        {
          return i;
        }
      }

      TALGA_WARN(0, std::string("attempted to find the offest of non exsistent texture in map ") + mName );
      return -1;
    }

    I32 EditorMap::getTileOffset(Tile t) const
    {
      I32 texOffset = getOffset(t.first);

      I32 x = t.second[0](0) * t.first->w() / mTileWidth;
      I32 y = t.second[0](1) * t.first->h() / mTileHeight;

      return texOffset + y * (t.first->w() / mTileWidth) + x;
    }

    void EditorMap::insertSheet(cpTex tex)
    {
      bool found = false;
      for (auto iter = mTileSet.begin(); iter != mTileSet.end(); ++iter)
      {
        if (iter->first == tex) return;
      }

      for (I32 y = 0; y < tex->h(); y += mTileHeight)
      {
        for (I32 x = 0; x < tex->w(); x += mTileWidth)
        {
          vec2 uvBL = GET_UV( x, tex->w(), y, tex->h());
          vec2 uvBR = GET_UV( x + mTileWidth, tex->w(), y, tex->h());
          vec2 uvTR = GET_UV( x + mTileWidth, tex->w(),y + mTileHeight, tex->h());
          vec2 uvTL = GET_UV( x, tex->w(), y + mTileHeight, tex->h());

          mTileSet.push_back( Tile{tex, UVFrame{{uvTL, uvTR, uvBR, uvBL}}} );
        }
      }

      TALGA_MSG(tex->getName() + " was sucessfully added to the map " + mName);

      return;
    }

    void EditorMap::insertIndices(std::vector<Tile> tiles, std::vector<iPnt> drop)
    {
      //I32 y = drop.y;
      I32 count = 0;
      for (const auto& t : tiles)
      {
        mMap[drop[count].y() * mWidth + drop[count].x()] = getTileOffset(t);
        ++count;
      }
    }

    IndicesList EditorMap::insertTile(std::vector<iPnt> selection, iPnt dropPos, cpTex tex)
    {
      insertSheet(tex);

      I32 offset = getOffset(tex);
      IndicesList previousIndices;

      I32 greatestX = 0;
      I32 greatestY = 0;
      I32 smallestX = 100000;
      I32 smallestY = 100000;
      I32 greatestXIndex = 0;
      I32 greatestYIndex = 0;
      I32 smallestXIndex = 0;
      I32 smallestYIndex = 0;
      I32 count = 0;

      if (selection.size() > 0)
      {
        for (const auto& pnt : selection)
        {
          if (pnt.x() > greatestX)
          {
            greatestX = pnt.x();
            greatestXIndex = count;
          }

          if (pnt.y() > greatestY)
          {
            greatestY = pnt.y();
            greatestYIndex = count;
          }

          if (pnt.x() < smallestX)
          {
            smallestX = pnt.x();
            smallestXIndex = pnt.x();
          }

          if (pnt.y() < smallestY)
          {
            smallestY = pnt.y();
            smallestYIndex = pnt.y();
          }

          ++count;
        }
      }

      for (const auto& iPnt : selection)
      {
          if (!Exists((I32)dropPos.x() + iPnt.x(), (I32)dropPos.y() + iPnt.y())) continue;

          previousIndices.push_back(mTileSet[mMap[ (I32)dropPos.y() * mWidth + (I32)dropPos.x()] - 1]);

          I32 index = offset + ( iPnt.y() * (tex->w() / mTileWidth) + iPnt.x()) + 1;

          mMap[ ((I32)dropPos.y() + iPnt.y() - smallestY) * mWidth + ((I32)dropPos.x() + (I32)iPnt.x() - smallestX)] = index;
      }

      return previousIndices;
    }

    Tile EditorMap::getTile(I32 x, I32 y, cpTex tex)
    {
      I32 offset = getOffset(tex);
      TALGA_ASSERT(offset != -1, "attempted to get offset of nonexistent texture");

      I32 textureTileWidth = tex->w() / mTileWidth;
      I32 textureTileHeight = tex->h() / mTileHeight;

      TALGA_ASSERT( (x >= 0 && x < textureTileWidth) && (y >= 0 && y < textureTileHeight), "");

      return mTileSet[offset + (y * textureTileWidth + x)];
    }

    Tile EditorMap::insertTile(const Tile &tile, const iPnt &drop)
    {
        Tile previousTile;
        if (!Exists(drop.x(), drop.y())) return previousTile;

        previousTile = mTileSet[mMap[drop.y() * mWidth + drop.x()]];

        mMap[drop.y() * mWidth + drop.x()] = getTileOffset(tile);

        return previousTile;
    }

    std::vector<Tile> EditorMap::getTiles(std::vector<iPnt> tiles, cpTex tex)
    {
      std::vector<Tile> set;

      for (const auto& pnt : tiles)
      {
        set.push_back(getTile(pnt.x(), pnt.y(), tex));
      }

      return set;
    }

  }
}
