#include "cchangeworkinglayer.h"


namespace talga
{
namespace editor
{

CChangeWorkingLayer::CChangeWorkingLayer(std::string layerName, EditorMap* map)
  : mLayerName(layerName)
  , mMap(map)
  , mPreviousLayerName("broken")
{
  setText(QString( "Working layer: " + QString::fromStdString(layerName)));
}

void CChangeWorkingLayer::redo()
{
  mMap->setWorkingLayer(mLayerName);
}

void CChangeWorkingLayer::undo()
{
  mPreviousLayerName = mMap->setWorkingLayer(mPreviousLayerName);
}

}
}
