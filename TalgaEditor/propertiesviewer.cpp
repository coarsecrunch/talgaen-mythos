#include "propertiesviewer.h"
#include "editormap.h"
#include <QFont>

namespace talga
{
namespace editor
{
  
PropertiesViewer::PropertiesViewer(QWidget* parent)
  : QTreeWidget(parent)
  , mAsset{nullptr}
  , mAssetType(NONE)
{
  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(sl_itemDoubleClicked(QTreeWidgetItem*,int)));
}

void PropertiesViewer::sl_assetSelected(AAsset* asset)
{
  QFont unmutableFont;
  unmutableFont.setBold(true);
  unmutableFont.setItalic(true);


  if (dynamic_cast<EditorMap*>(asset))
  {
    EditorMap* map = static_cast<EditorMap*>(asset);
    mAssetType = MAP;
    clear();

    QTreeWidgetItem* tileWidth = new QTreeWidgetItem(this);
    tileWidth->setText(0, "tileWidth");
    tileWidth->setText(1, QString::number(map->getTileWidth()));
    tileWidth->setFont(0, unmutableFont);
    tileWidth->setFont(1, unmutableFont);

    QTreeWidgetItem* tileHeight = new QTreeWidgetItem(this);
    tileHeight->setText(0, "tileHeight");
    tileHeight->setText(1,QString::number(map->getTileHeight()));
    tileHeight->setFont(0, unmutableFont);
    tileHeight->setFont(1, unmutableFont);

    QTreeWidgetItem* width = new QTreeWidgetItem(this);
    width->setText(0, "width");
    width->setText(1,QString::number(map->getWidth()));

    QTreeWidgetItem* height = new QTreeWidgetItem(this);
    height->setText(0, "height");
    height->setText(1,QString::number(map->getHeight()));

    headerItem()->setText(0, "map");
  }
}

void PropertiesViewer::sl_itemDoubleClicked(QTreeWidgetItem *itm, int col)
{
  if (col == 0)
  {
    itm->setFlags(itm->flags() & ~Qt::ItemIsEditable);
    return;
  }

  QString text = itm->text(0);

  switch (mAssetType)
  {
  case NONE:
    break;
  case MAP:
  {
    if (text == "width" || text == "height")
    {
      itm->setFlags(itm->flags() | Qt::ItemIsEditable);
    }
  }break;

  default:
    break;
  }
}

}
}
