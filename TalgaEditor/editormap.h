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
      Tile getTileAtSheet(I32 x, I32 y, cpTex tex);
      void insertSheet(cpTex tex);

      IndicesList insertTile(const std::vector<iPnt>& dropPositions, const std::vector<Tile>& tiles, I32 layerIndex);
      std::vector<Tile> getTiles(std::vector<iPnt> tiles, cpTex tex);
    };

  }
}

#endif // EDITORMAP_H
