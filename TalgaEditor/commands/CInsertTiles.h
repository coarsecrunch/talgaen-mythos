#pragma once

#include <QUndoCommand>
#include <vector>
#include "Cmn.h"
#include "Rect.h"
#include "editormap.h"
#include "AssetManager.h"

namespace talga
{
  namespace editor
  {
    class CInsertTiles : public QUndoCommand
    {
    public:
      CInsertTiles(EditorMap* map, std::vector<Tile> tiles, iPnt drop, std::vector<iPnt> selection);

      virtual ~CInsertTiles() {}

      virtual void undo() override;
      virtual void redo() override;
    private:
      EditorMap* mMap;

      // both should be exactly the same size
      std::vector<Tile> mTiles;
      std::vector<Tile> mPreviousTiles;

      // should be a list of tiles IN MAP SPACE NOT TEXTURE SPACE
      std::vector<iPnt> mDropPositions;

    };
  }
}
