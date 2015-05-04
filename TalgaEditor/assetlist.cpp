#include "assetlist.h"

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>

namespace talga
{
namespace editor
{

AssetList::AssetList(QWidget* parent)
    : QListWidget(parent)
{
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
        QImage image(str);
        mAssets.insert(fileName, new QPixmap(QPixmap::fromImage(image)));

        addItem(fileName);
    }
}

void AssetList::sl_assetSelected(QListWidgetItem* item)
{
    emit sig_textureSelected(mAssets[item->text()]);
}



}
}
