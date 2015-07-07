#include "caddlayer.h"
#include "layerstack.h"
namespace talga
{
namespace editor
{
CAddLayer::CAddLayer(EditorMap* map, std::string name, LayerStack* stack, I32 index)
  : mMap(map)
  , mName(name)
  , mRemovedLayer()
  , mStack(stack)
  , mIndex(index)
{
  setText("new layer added");
}

void CAddLayer::undo()
{
  mRemovedLayer = mMap->deleteLayer(mName);
  mStack->sl_removeLayerWidget(mName);
}

void CAddLayer::redo()
{
  mMap->addLayer(mName, mIndex);
  mStack->sl_addLayerWidget(mName, mIndex);
}

}
}
