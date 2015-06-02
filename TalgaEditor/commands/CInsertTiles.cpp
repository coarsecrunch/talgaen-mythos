#include "CInsertTiles.h"
#include "editormap.h"
#include "Texture.h"
#include "Point.h"

namespace talga
{
  namespace editor
  {
    CInsertTiles::CInsertTiles(EditorMap* map, std::vector<Tile> tiles, Point drop, std::vector<Point> selection)
      : mMap(map)
      , mTiles(tiles)
      , mSelection(selection)
      , mPreviousTiles()
    {
      TALGA_ASSERT( tiles.size() == selection.size(), "passed incorrect Rect to CInsertTiles");
      TALGA_ASSERT( map, "");

      setText("tile insert");
    }

    void CInsertTiles::undo()
    {
      std::vector<Point> shiftedPoints;

      for (const auto& pnt : mSelection)
      {
        shiftedPoints.push_back(Point(pnt.x() + mDrop.x(), pnt.y() + mDrop.y()));
      }

      mMap->insertIndices(mPreviousTiles, shiftedPoints);
    }

    void CInsertTiles::redo()
    {
      I32 tW = mMap->getTileWidth();
      I32 tH = mMap->getTileHeight();

      if (mDrop.x() * tW >= 0 && mDrop.x() < mMap->getTileWidth() * mMap->getWidth()
          && mDrop.y() * tH >= 0 && mDrop.y() * tH < mMap->getTileHeight() * mMap->getHeight())
      {
        mPreviousTiles = mMap->insertTile(mSelection, mDrop, mTiles[0].first);
      }

    }


  }
}
