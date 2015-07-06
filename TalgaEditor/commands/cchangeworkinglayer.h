#pragma once

#include "Cmn.h"
#include <QUndoCommand>
#include <string>
#include "editormap.h"
namespace talga
{
namespace editor
{
class CChangeWorkingLayer : public QUndoCommand
{
public:
  CChangeWorkingLayer(std::string layerName, EditorMap* map);

  virtual void redo() override;
  virtual void undo() override;
private:
  std::string mLayerName;
  std::string mPreviousLayerName;
  EditorMap* mMap;
};

}
}
