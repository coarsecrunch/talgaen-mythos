#include "layerstack.h"
#include <QTreeWidgetItem>

namespace talga
{
namespace editor
{

LayerStack::LayerStack(QWidget* parent)
  : QTreeWidget(parent)
  , mLayers(nullptr)
  , mCurrentLayerIndex(0)
{

}

void LayerStack::sl_updateLayerStack(std::vector<MapLayer> *layers)
{
  clear();

  for (auto iter = layers->begin(); iter != layers->end(); ++iter)
  {
    QTreeWidgetItem* itm = new QTreeWidgetItem( this);
    itm->setText(0, QString::fromStdString( (*iter).getName() ) );
    itm->setCheckState(1, Qt::Checked);
  }
}

}
}
