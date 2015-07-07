#pragma once

#include <QUndoCommand>
#include "editormap.h"
#include "layerstack.h"

namespace talga
{
namespace editor
{

class CRemoveLayer : public QUndoCommand
{
public:
  CRemoveLayer(EditorMap* map, std::string name, LayerStack* stack, I32 index);

  virtual void undo() override;
  virtual void redo() override;
private:
  EditorMap* mMap;
  std::string mName;
  MapLayer mRemovedLayer;
  LayerStack* mStack;
  I32 mIndex;
};

}
}
