#include "assetlist.h"

#include <QObject>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QDrag>
#include <QMimeData>
#include <QMessageBox>

#include "Cmn.h"
#include "editormap.h"
#include "Texture.h"
#include "gdata.h"
#include "AssetManager.h"
#include "sys.h"

namespace talga
{
  namespace editor
  {

    AssetList::AssetList(QWidget* parent)
      : QTreeWidget(parent)
      , mTexturesFolder(nullptr)
      , mMapsFolder(nullptr)
      , mScriptsFolder(nullptr)
      , mLevelFolder{nullptr}
      , mMap{nullptr}
      , mAssets{}
    {
    }

    AssetList::~AssetList()
    {
    }

    void AssetList::sl_chooseAssets()
    {
      QFileDialog dialog;
      dialog.setFileMode(QFileDialog::ExistingFiles);
      dialog.setNameFilter(tr("Textures (*.png)"));
      QStringList filePaths;

      if(dialog.exec())
        filePaths = dialog.selectedFiles();

      foreach(QString str, filePaths)
      {
        sl_loadAsset(str);
      }
    }

    void AssetList::sl_loadAsset(QString path)
    {
      QString fileName = QFileInfo(QFile(path).fileName()).fileName();
      QString fileExtension = QFile(path).fileName().split(".").at(QFile(path).fileName().split(".").size() - 1);
      cpAsset asset = GData::getInstance()->getManager()->AddAsset(path.toStdString());

      if (!asset) return;

      mAssets.push_back(asset);

      if (fileExtension == "png")
      {
        QTreeWidgetItem* txt = new QTreeWidgetItem(mTexturesFolder);
        txt->setText(0, fileName);

        emit sig_assetChosen(path);
      }
      else
      {
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString("invalid file"));
        msgBox.exec();
      }
    }

    void AssetList::sl_updateChangedMap(EditorMap * map)
    {
      mMap = map;
      if (mLevelFolder)
        delete mLevelFolder;

      mAssets.clear();

      if (map)
      {
        mLevelFolder = new QTreeWidgetItem(this);
        mLevelFolder->setText(0, QString::fromStdString(map->getName()));

        mTexturesFolder = new QTreeWidgetItem(mLevelFolder);
        mTexturesFolder->setText(0, "tile sheets");
        mScriptsFolder = new QTreeWidgetItem(mLevelFolder);
        mScriptsFolder->setText(0, "scripts");

        for (auto it = map->getTileSheets().begin(); it != map->getTileSheets().end(); ++it)
        {
          QTreeWidgetItem* temp = new QTreeWidgetItem(mTexturesFolder);
          temp->setText(0, QString::fromStdString( (*it)->getName()));
          mAssets.insert(QString::fromStdString((*it)->getName()), new QImage(QString::fromStdString((*it)->getPath() + (*it)->getName())));
        }
      }

    }

    void AssetList::sl_updateAssetsNames()
    {
      for (auto it = mAssets.begin(); it != mAssets.end(); ++it)
      {
        if ( getFileExtension((*it)->getName()) == ".png" )
      }
    }

    void AssetList::sl_assetSelected(QTreeWidgetItem* item, int column)
    {
      if (item == mTexturesFolder || item == mMapsFolder || item == mScriptsFolder)
        return;

      if (item == mLevelFolder)
      {
        emit sig_assetSelected((EditorMap*)GData::getInstance()->getCurrentMap());
        return;
      }

      emit sig_textureSelected(TextureAsset(item->text(column), mAssets[item->text(column)]));
    }

    void AssetList::mousePressEvent(QMouseEvent *e)
    {
      if (e->button() == Qt::LeftButton)
        dragStartPosition = e->pos();

      QTreeWidget::mousePressEvent(e);
    }

    void AssetList::mouseMoveEvent(QMouseEvent *e)
    {
      if (!(e->buttons() & Qt::LeftButton))
        return;
      if ( (e->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance() )
        return;

      auto item = itemAt(dragStartPosition);

      if (!item)
        return;

      QDrag* drag = new QDrag(this);
      QMimeData* mimeData = new QMimeData;

      mimeData->setText(item->text(0));
      drag->setMimeData(mimeData);
      Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
    }

  }
}
