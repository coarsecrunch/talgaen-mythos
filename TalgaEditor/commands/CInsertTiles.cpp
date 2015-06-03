#include "CInsertTiles.h"
#include "editormap.h"
#include "Texture.h"
#include "Point.h"

namespace talga
{
  namespace editor
  {
    CInsertTiles::CInsertTiles(EditorMap* map, std::vector<Tile> tiles, iPnt drop, std::vector<iPnt> selection)
      : mMap(map)
      , mTiles(tiles)
      , mPreviousTiles()
    {
      TALGA_ASSERT( tiles.size() == selection.size(), "passed incorrect Rect to CInsertTiles");
      TALGA_ASSERT( map, "");

      for (const auto& tile : selection)
      {
          mDropPositions.emplace_back(drop.x() + tile.x(), drop.y() + tile.y());
      }

      setText("tile insert");
    }

    void CInsertTiles::undo()
    {
     /* std::vector<Point> shiftedPoints;

      for (const auto& pnt : mSelection)
      {
        shiftedPoints.push_back(Point(pnt.x() + mDrop.x(), pnt.y() + mDrop.y()));
      }

      mMap->insertIndices(mPreviousTiles, shiftedPoints);
    */}

    void CInsertTiles::redo()
    {
      I32 tW = mMap->getTileWidth();
      I32 tH = mMap->getTileHeight();

      auto dropIter = mDropPositions.begin();

      for (auto tileIter = mTiles.begin(); tileIter != mTiles.end(); ++tileIter, ++dropIter)
      {
          mMap->insertTile(*tileIter, *dropIter);
      }

      /*if (mDrop.x() * tW >= 0 && mDrop.x() < mMap->getTileWidth() * mMap->getWidth()
          && mDrop.y() * tH >= 0 && mDrop.y() * tH < mMap->getTileHeight() * mMap->getHeight())
      {
        mPreviousTiles = mMap->insertTile(mSelection, mDrop, mTiles[0].first);
      }*/

    }


  }
}
