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

    typedef std::vector<Tile> IndicesList;
    class EditorMap : public Map
    {
    public:
      EditorMap();
      EditorMap(const EditorMap& cpy);
      virtual ~EditorMap();

      const EditorMap& operator=(const EditorMap& cpy);
      const EditorMap& operator=(const Map& cpy);

      I32 getOffset(cpTex tex) const;
      I32 getTileOffset(Tile t) const;
      Tile getTile(I32 x, I32 y, cpTex tex);
      void insertSheet(cpTex tex);

      IndicesList insertTile(std::vector<Point> selection, Point dropPos, cpTex tex);
      void insertIndices(std::vector<Tile> tiles, std::vector<Point> drop);
      std::vector<Tile> getTiles(std::vector<Point> tiles, cpTex tex);
    };

  }
}

#endif // EDITORMAP_H
