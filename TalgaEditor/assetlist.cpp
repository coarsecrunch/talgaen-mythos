#include "assetlist.h"

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include "wrongextdialog.h"

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
        QString fileName = QFileInfo(QFile(str).fileName()).fileName();
        QString fileExtension = QFile(str).fileName().split(".",QString::SkipEmptyParts).at(1);
        qDebug() << fileExtension;

        QImage image(str);
        mAssets.insert(fileName, new QPixmap(QPixmap::fromImage(image)));

        if (fileExtension == "png")
        {
            QTreeWidgetItem* txt = new QTreeWidgetItem(mTexturesFolder);
            txt->setText(0, fileName);
        }
        else
        {
            WrongExtDialog ext;
            ext.exec();
        }

    }
}

void AssetList::sl_assetSelected(QTreeWidgetItem* item, int column)
{
    if (item == mTexturesFolder || item == mMapsFolder || item == mScriptsFolder)
        return;

    emit sig_textureSelected(mAssets[item->text(0)]);
}



}
}
