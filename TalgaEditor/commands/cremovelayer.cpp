#include "cremovelayer.h"

namespace talga
{
namespace editor
{
CRemoveLayer::CRemoveLayer(EditorMap* map, std::string name, LayerStack* stack, I32 index)
  : mMap(map)
  , mName(name)
  , mStack(stack)
  , mIndex(index)
{
  setText(QString::fromStdString(name) + " removed");
}

void CRemoveLayer::undo()
{
  mMap->addLayer(mRemovedLayer, mIndex);
  mStack->sl_addLayerWidget(mName, mIndex);
}

void CRemoveLayer::redo()
{
   mRemovedLayer = mMap->deleteLayer(mName);
   mStack->sl_removeLayerWidget(mName);
}

}
}
