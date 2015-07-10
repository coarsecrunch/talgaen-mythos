#include "layerstack.h"
#include <QTreeWidgetItem>
#include <QDebug>
#include "commands/cchangeworkinglayer.h"
#include "commands/caddlayer.h"
#include "commands/cremovelayer.h"
#include "commands/cchangelayerorder.h"
#include <algorithm>

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

void LayerStack::sl_updateChangedMap(EditorMap* map)
{
  clear();
  mMap = map;
  if (map)
  {
    for (auto iter = mMap->getLayers()->begin(); iter != mMap->getLayers()->end(); ++iter)
    {
      sl_addLayerWidget( iter->getName(), topLevelItemCount() );
    }
  }
}

void LayerStack::sl_updateItemChanged(QTreeWidgetItem *itm, int column)
{
  //name
  if (column == 0)
  {
    for (auto itr = mMap->getLayers()->begin();itr != mMap->getLayers()->end(); ++itr)
    {
      if ( itm->text(0).toStdString() == itr->getName() || itm->text(0) == "" || itm->text(0).contains(' '))
      {
        itm->setText(0, QString::fromStdString((*mMap->getLayers())[indexOfTopLevelItem(itm)].getName()));
        TALGA_WARN(0, "attempted to set invalid layer name");
        return;
      }
    }
    (*mMap->getLayers())[indexOfTopLevelItem(itm)].setName(itm->text(column).toStdString());
  }
  //Visibility
  else if (column == 1)
  {
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
}

void LayerStack::sl_addLayer()
{
  if (!mMap) return;

  std::string name = std::string("layer") + std::to_string(topLevelItemCount());

  emit sig_addUndoCommand(new CAddLayer(mMap, name, this, mMap->getLayers()->size()));
}

void LayerStack::sl_removeLayer()
{
  if(mMap && mMap->getWorkingLayer())
  {
    std::string previousName = mMap->getWorkingLayer()->getName();
    emit sig_addUndoCommand(new CRemoveLayer(mMap, previousName, this, mMap->getWorkingLayerIndex()));
  }
}

void LayerStack::sl_addLayerWidget(std::string name, I32 idx)
{
  QTreeWidgetItem* itm = new QTreeWidgetItem();

  itm->setFlags(itm->flags() | Qt::ItemIsEditable);
  itm->setText(0, QString::fromStdString(name));
  itm->setCheckState(1, Qt::Checked);

  insertTopLevelItem(idx, itm);
  //sl_changeWorkingLayer(itm, 0);
}

void LayerStack::sl_removeLayerWidget(std::string name)
{
  QTreeWidgetItemIterator it(this);
  while (*it)
  {
    std::string tempStr = (*it)->text(0).toStdString();

    if (name == (*it)->text(0).toStdString())
    {

      mMap->setWorkingLayer("null");

      delete *it;
      return;
    }

    ++it;
  }
}

void LayerStack::sl_swapLayerWidgets(I32 idx1, I32 idx2)
{
  QTreeWidgetItem* temp = new QTreeWidgetItem();
  *temp = *topLevelItem(idx1);
  *topLevelItem(idx1) = *topLevelItem(idx2);
  *topLevelItem(idx2) = *temp;

  addTopLevelItem(temp);
  delete temp;
}

void LayerStack::sl_moveWorkingLayerDown()
{
  if (mMap && mMap->getWorkingLayer() && mMap->getWorkingLayerIndex() < mMap->getLayers()->size() - 1)
  {
    emit sig_addUndoCommand(new CChangeLayerOrder(mMap, this, CChangeLayerOrder::DOWN));
    emit sig_updateGL();
  }
}

void LayerStack::sl_moveWorkingLayerUp()
{
  if (mMap && mMap->getWorkingLayer() && mMap->getWorkingLayerIndex() > 0)
  {
    emit sig_addUndoCommand(new CChangeLayerOrder(mMap, this, CChangeLayerOrder::UP));
    emit sig_updateGL();
  }
}

void LayerStack::sl_changeWorkingLayer(QTreeWidgetItem *itm, int column)
{
  if (mMap->getWorkingLayer())
  {
    if (itm->text(0).toStdString() == mMap->getWorkingLayer()->getName()) return;
  }

  emit sig_addUndoCommand(new CChangeWorkingLayer(itm->text(0).toStdString(), mMap));
}

}
}
