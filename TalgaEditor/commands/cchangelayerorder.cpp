#include "cchangelayerorder.h"

namespace talga
{
namespace editor
{

CChangeLayerOrder::CChangeLayerOrder(EditorMap* map, LayerStack* stack, MOVE direction)
  : mMap(map)
  , mStack(stack)
  , mDirection{direction}
  , mWorkingLayerPreviousPos{-1}

{
  if (direction == UP)
  {
    setText(QString::fromStdString(std::string("move ") + map->getWorkingLayer()->getName() + " up"));
  }
  else
  {
    setText(QString::fromStdString(std::string("move ") + map->getWorkingLayer()->getName() + std::string(" down")));
  }
}

void CChangeLayerOrder::undo()
{
  I32 mWorkingLayerPreviousPos = mMap->getWorkingLayerIndex();
  std::string workingLayerName = mMap->getWorkingLayer()->getName();

  MapLayer temp;
  if (mDirection == UP)
  {
    temp = mMap->getLayers()->at(mMap->getWorkingLayerIndex() + 1);
    mMap->getLayers()->at(mMap->getWorkingLayerIndex() + 1) = *mMap->getWorkingLayer();
    mMap->getLayers()->at(mWorkingLayerPreviousPos) = temp;
    mMap->setWorkingLayer(workingLayerName);
    mStack->sl_swapLayerWidgets(mWorkingLayerPreviousPos, mMap->getWorkingLayerIndex());
  }
  else
  {
    temp = mMap->getLayers()->at(mMap->getWorkingLayerIndex() - 1);
    mMap->getLayers()->at(mMap->getWorkingLayerIndex() - 1) = *mMap->getWorkingLayer();
    mMap->getLayers()->at(mWorkingLayerPreviousPos) = temp;
    mMap->setWorkingLayer(workingLayerName);
    mStack->sl_swapLayerWidgets(mWorkingLayerPreviousPos, mMap->getWorkingLayerIndex());
  }
}

void CChangeLayerOrder::redo()
{
  I32 mWorkingLayerPreviousPos = mMap->getWorkingLayerIndex();
  std::string workingLayerName = mMap->getWorkingLayer()->getName();

  MapLayer temp;
  if (mDirection == UP)
  {
    temp = mMap->getLayers()->at(mMap->getWorkingLayerIndex() - 1);
    mMap->getLayers()->at(mMap->getWorkingLayerIndex() - 1) = *mMap->getWorkingLayer();
    mMap->getLayers()->at(mWorkingLayerPreviousPos) = temp;
    mMap->setWorkingLayer(workingLayerName);
    mStack->sl_swapLayerWidgets(mWorkingLayerPreviousPos, mMap->getWorkingLayerIndex());
  }
  else
  {
    temp = mMap->getLayers()->at(mMap->getWorkingLayerIndex() + 1);
    mMap->getLayers()->at(mMap->getWorkingLayerIndex() + 1) = *mMap->getWorkingLayer();
    mMap->getLayers()->at(mWorkingLayerPreviousPos) = temp;
    mMap->setWorkingLayer(workingLayerName);
    mStack->sl_swapLayerWidgets(mWorkingLayerPreviousPos, mMap->getWorkingLayerIndex());
  }
}

}
}
