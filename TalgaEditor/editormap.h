#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"
#include "Cmn.h"

namespace talga
{

  class AssetManager;
  class Rect;

  namespace editor
  {

    typedef std::vector<std::vector<I32>> IndicesList;
    class EditorMap : public Map
    {
    public:
      EditorMap();
      EditorMap(const EditorMap& cpy);
      virtual ~EditorMap();

      const EditorMap& operator=(const EditorMap& cpy);
      const EditorMap& operator=(const Map& cpy);

      I32 getOffset(cpTex tex);
      Tile getTile(I32 x, I32 y, cpTex tex);
      void insertSheet(cpTex tex);

      IndicesList insertTile(Rect selection, Rect dropPos, cpTex tex);
      void insertIndices(IndicesList tiles, Rect drop);
      std::vector<Tile> getTiles(Rect tiles, cpTex tex);
    };

  }
}

#endif // EDITORMAP_H
