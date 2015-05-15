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

    I32 EditorMap::getOffset(cpTex tex)
    {
      for (I32 i = 0; i < mTileSet.size(); ++i)
      {
        if (tex == mTileSet[i].first)
        {
          return i + 1;
        }
      }

      TALGA_WARN(0, std::string("attempted to find the offest of non exsistent texture in map ") + mName );
      return -1;
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

      TALGA_MSG(tex->getName() + std::string(" was sucessfully added to the map ") + mName);

      return;
    }

    void EditorMap::insertTile(Rect selection, Rect dropPos, cpTex tex)
    {
      TALGA_ASSERT(Exists(selection.x, selection.y) && Exists(selection.x + selection.w, selection.y + selection.h), "Tried to insert tile in incorrect position");
      insertSheet(tex);

      I32 offset = getOffset(tex);

      for (I32 y = selection.y, countY = 0; y < selection.y + selection.h; ++y, ++countY)
      {
        for (I32 x = selection.x, countX = 0; x < selection.x + selection.w; ++x, ++countX)
        {
          mMap[ ((I32)dropPos.y + countY)* mWidth + ((I32)dropPos.x + countX)] = offset + (y * (tex->w() / mTileWidth) + x);
        }
      }
    }

  }
}
