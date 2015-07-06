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

      mDropPositions = selection;

      offsetTiles(mDropPositions, drop);
      setText("tile insert");
    }

    void CInsertTiles::undo()
    {
      mMap->insertTile(mDropPositions, mPreviousTiles);
    }

    void CInsertTiles::redo()
    {
       mPreviousTiles = mMap->insertTile(mDropPositions, mTiles);
    }

    void CInsertTiles::offsetTiles(std::vector<iPnt> &tilePositions, iPnt drop)
    {
      I32 greatestX = 0;
      I32 greatestY = 0;
      I32 smallestX = 1000000;
      I32 smallestY = 1000000;
      I32 greatestXIndex = 0;
      I32 greatestYIndex = 0;
      I32 smallestXIndex = 0;
      I32 smallestYIndex = 0;
      I32 count = 0;

      if (tilePositions.size() > 0)
      {
        for (const auto& pnt : tilePositions)
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
            smallestXIndex = count;
          }

          if (pnt.y() < smallestY)
          {
            smallestY = pnt.y();
            smallestYIndex = count;
          }

          ++count;
        }
      }

      for (auto& pnt : tilePositions)
      {
        pnt.setX(pnt.x() + drop.x() - smallestX);
        pnt.setY(pnt.y() + drop.y() - smallestY);
      }

    }


  }
}
