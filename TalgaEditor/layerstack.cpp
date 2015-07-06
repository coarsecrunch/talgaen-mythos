#include "layerstack.h"
#include <QTreeWidgetItem>
#include <QDebug>
#include "commands/cchangeworkinglayer.h"
namespace talga
{
namespace editor
{

LayerStack::LayerStack(QWidget* parent)
  : QTreeWidget(parent)
  , mMap(nullptr)
  , mCurrentLayerIndex(0)
{
}

void LayerStack::sl_updateLayerStack(EditorMap* map)
{
  clear();
  mMap = map;
  QTreeWidgetItem* topLayer = nullptr;
  for (auto iter = mMap->getLayers()->begin(); iter != mMap->getLayers()->end(); ++iter)
  {
    QTreeWidgetItem* itm = new QTreeWidgetItem(this);
    itm->setFlags(itm->flags() | Qt::ItemIsEditable);
    itm->setText(0, QString::fromStdString( (*iter).getName() ) );
    itm->setCheckState(1, Qt::Checked);
    topLayer = itm;
  }

  sl_changeWorkingLayer(topLayer, 0);
}

void LayerStack::sl_updateItemChanged(QTreeWidgetItem *itm, int column)
{
  int id = indexOfTopLevelItem(itm);
  if (column == 0 && itm->text(0) != "")
  {
    (*mMap->getLayers())[indexOfTopLevelItem(itm)].setName(itm->text(column).toStdString());
  }
  //Visibility
  else if (column == 1)
  {
    qDebug() << "old visibilty name: " <<(*mMap->getLayers())[indexOfTopLevelItem(itm)].isVisible();
    if (itm->checkState(column) == Qt::Checked)
    {
      (*mMap->getLayers())[indexOfTopLevelItem(itm)].setVisible(true);
    }
    else if (itm->checkState(column) == Qt::Unchecked)
    {
      (*mMap->getLayers())[indexOfTopLevelItem(itm)].setVisible(false);
    }
  }

  emit sig_updateGL();
  qDebug() << "new name: " << itm->text(column);
}

void LayerStack::sl_changeWorkingLayer(QTreeWidgetItem *itm, int column)
{
  if (indexOfTopLevelItem(itm) == mMap->getWorkingLayerIndex()) return;

  emit sig_addUndoCommand(new CChangeWorkingLayer(itm->text(0).toStdString(), mMap));
}

}
}
