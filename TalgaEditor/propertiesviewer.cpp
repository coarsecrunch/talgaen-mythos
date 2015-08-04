#include "propertiesviewer.h"
#include "editormap.h"
#include <QFont>
#include <QFileDialog>
#include "sys.h"

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
  connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(sl_itemChanged(QTreeWidgetItem*,int)));
}

void PropertiesViewer::sl_assetSelected(AAsset* asset)
{
  QFont unmutableFont;
  unmutableFont.setBold(true);
  unmutableFont.setItalic(true);
  clear();

  if (dynamic_cast<EditorMap*>(asset))
  {
    EditorMap* map = static_cast<EditorMap*>(asset);
    mAssetType = MAP;
    mAsset = asset;

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

    QTreeWidgetItem* initScript = new QTreeWidgetItem(this);
    initScript->setText(0, "initScript");

    if (map->getScript())
    {
      initScript->setText(1, QString::fromStdString(map->getScript()->getPath() + map->getScript()->getName()));
      cpScript script = map->getScript();
    }
    else
    {
      initScript->setText(1, "");
    }
    headerItem()->setText(0, "map");
  }
  else
  {
    mAsset = nullptr;
  }
}

void PropertiesViewer::sl_updateChangedMap(EditorMap *)
{
  clear();
  mAsset = nullptr;
  mAssetType = NONE;
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
    EditorMap* map = static_cast<EditorMap*>(mAsset);
    if (text == "width" || text == "height")
    {
      itm->setFlags(itm->flags() | Qt::ItemIsEditable);
    }
    else if (text == "initScript")
    {
      QString file = QFileDialog::getOpenFileName(
            this,
            "select init lua script",
            "/home",
            "lua script (*.lua)");
      if (file == "") return;

      itm->setText(1, file);
      map->setInitScriptPath(file.toStdString());
    }
  }break;

  default:
    break;
  }
}

void PropertiesViewer::sl_itemChanged(QTreeWidgetItem *itm, int col)
{
  if (itm->text(1) == "" || itm->text(1) == " ") return;

  if (mAssetType == MAP)
  {
    EditorMap* map = static_cast<EditorMap*>(mAsset);
    if (itm->text(0) == "width")
    {
      map->setWidth(itm->text(1).toInt());
    }
    else if (itm->text(0) == "height")
    {
      map->setHeight(itm->text(1).toInt());
    }

    emit sig_updateGL();
  }


}

}
}
