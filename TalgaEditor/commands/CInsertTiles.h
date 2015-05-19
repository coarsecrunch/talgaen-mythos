#pragma once

#include <QUndoCommand>
#include <vector>
#include "Cmn.h"
#include "Rect.h"
#include "editormap.h"

namespace talga
{
  namespace editor
  {
    class CInsertTiles : public QUndoCommand
    {
    public:
      CInsertTiles(EditorMap* map, std::vector<Tile> tiles, Rect drop, Rect selection);

      virtual void undo() override;
      virtual void redo() override;
    private:
      EditorMap* mMap;
      std::vector<Tile> mTiles;
      Rect mDrop;
      Rect mSelection;
      IndicesList mPreviousTiles;
    };
  }
}
