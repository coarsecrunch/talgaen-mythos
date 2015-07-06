#include "assetlist.h"

#include <QObject>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QDrag>
#include <QMimeData>
#include "wrongextdialog.h"

#include "Cmn.h"

namespace talga
{
  namespace editor
  {

    AssetList::AssetList(QWidget* parent)
      : QTreeWidget(parent)
      , mTexturesFolder(nullptr)
      , mMapsFolder(nullptr)
      , mScriptsFolder(nullptr)
    {
      mTexturesFolder = new QTreeWidgetItem(this);
      mTexturesFolder->setText(0, "textures");
      mMapsFolder = new QTreeWidgetItem(this);
      mMapsFolder->setText(0, "maps");
      mScriptsFolder = new QTreeWidgetItem(this);
      mScriptsFolder->setText(0, "scripts");
    }

    AssetList::~AssetList()
    {
      qDeleteAll(mAssets);
    }

    void AssetList::sl_chooseAssets()
    {
      QFileDialog dialog;
      dialog.setFileMode(QFileDialog::ExistingFiles);
      dialog.setNameFilter(tr("Images (*.png *.jpg)"));
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

      mAssets.insert(fileName, new QImage(path));

      if (fileExtension == "png")
      {
        QTreeWidgetItem* txt = new QTreeWidgetItem(mTexturesFolder);
        txt->setText(0, fileName);

        emit sig_assetChosen(path);
      }
      else
      {
        WrongExtDialog ext;
        ext.exec();
      }
    }

    void AssetList::sl_assetSelected(QTreeWidgetItem* item, int column)
    {
      if (item == mTexturesFolder || item == mMapsFolder || item == mScriptsFolder)
        return;

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
