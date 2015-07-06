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
  setText(QString( "working layer: " + QString::fromStdString(layerName)));
}

void CChangeWorkingLayer::redo()
{
  mPreviousLayerName = mMap->setWorkingLayer(mLayerName);
}

void CChangeWorkingLayer::undo()
{
  mMap->setWorkingLayer(mPreviousLayerName);
}

}
}
