#pragma once

#include <QUndoCommand>
#include "editormap.h"
#include "layerstack.h"
namespace talga
{
namespace editor
{

class CAddLayer : public QUndoCommand
{
public:
  CAddLayer(EditorMap* map, std::string name, LayerStack* stack, I32 mIndex);

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
