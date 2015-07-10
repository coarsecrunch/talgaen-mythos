#pragma once

#include <QUndoCommand>
#include "editormap.h"
#include "layerstack.h"

namespace talga
{
namespace editor
{

class CChangeLayerOrder : public QUndoCommand
{
public:
  enum MOVE {UP, DOWN};

  CChangeLayerOrder(EditorMap* map, LayerStack* stack, MOVE direction);

  virtual void undo() override;
  virtual void redo() override;


private:
  EditorMap* mMap;
  LayerStack* mStack;
  I32 mWorkingLayerPreviousPos;
  MOVE mDirection;
};

}
}
