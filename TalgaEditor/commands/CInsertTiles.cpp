#include "CInsertTiles.h"
#include "editormap.h"
#include "Texture.h"

namespace talga
{
  namespace editor
  {
    CInsertTiles::CInsertTiles(EditorMap* map, std::vector<Tile> tiles, Rect drop, Rect selection)
      : mMap(map)
      , mTiles(tiles)
      , mDrop(drop)
      , mSelection(selection)
      , mPreviousTiles()
    {
      TALGA_ASSERT( tiles.size() == selection.w * selection.h, "passed incorrect Rect to CInsertTiles");
      setText("tile insert");
    }

    void CInsertTiles::undo()
    {
      mMap->insertIndices(mPreviousTiles, mDrop);
    }

    void CInsertTiles::redo()
    {
      I32 tW = mMap->getTileWidth();
      I32 tH = mMap->getTileHeight();

      if (mDrop.x * tW >= 0 && mDrop.x < mMap->getTileWidth() * mMap->getWidth()
          && mDrop.y * tH >= 0 && mDrop.y * tH < mMap->getTileHeight() * mMap->getHeight())
      {
        mPreviousTiles = mMap->insertTile(mSelection, Rect{mDrop.x, mDrop.y}, mTiles[0].first);
      }

    }


  }
}
