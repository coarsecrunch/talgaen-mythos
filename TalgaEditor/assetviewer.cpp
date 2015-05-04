#include "assetviewer.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
namespace talga
{
namespace editor
{

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

AssetViewer::AssetViewer(QWidget* parent)
    : QGraphicsView(parent)
    , pmImageViewScene(new QGraphicsScene)
{
    setScene(pmImageViewScene);

    static QPen lineProperties(Qt::black);
    lineProperties.setWidth(3);

    mSelectBox = new QGraphicsRectItem( 0, 0, 0 + TILE_WIDTH, 0 + TILE_HEIGHT);
    mSelectBox->setPen(lineProperties);

    pmImageViewScene->addItem(mSelectBox);
}

AssetViewer::~AssetViewer()
{
    pmImageViewScene->clear();
    delete pmImageViewScene;
}

void AssetViewer::mouseMoveEvent(QMouseEvent* e)
{
    updateBox(e->x(), e->y());
}

void AssetViewer::sl_updateTexture(QPixmap* pix)
{
    pmImageViewScene->removeItem(mSelectBox);
    pmImageViewScene->clear();


    static QPen lineProperties(Qt::gray);
    lineProperties.setWidth(1);

    pmImageViewScene->addItem(new QGraphicsPixmapItem(*pix));

    int imageWidth = pix->width();
    int imageHeight = pix->height();

    for (int x = 0; x <= imageWidth / TILE_WIDTH; x++ )
    {
        pmImageViewScene->addLine(x * TILE_WIDTH, 0, x * TILE_WIDTH, imageHeight, lineProperties);
    }

    for (int y = 0; y <= imageHeight / TILE_HEIGHT; y++)
    {
        pmImageViewScene->addLine(0, y * TILE_HEIGHT, imageWidth, y * TILE_HEIGHT, lineProperties);
    }

    pmImageViewScene->addItem(mSelectBox);
}

void AssetViewer::updateBox(int mX, int mY)
{
    int boxX = mX - mX % TILE_WIDTH;
    int boxY = mY - mY % TILE_HEIGHT;

    mSelectBox->setX(boxX);
    mSelectBox->setY(boxY);
}



}
}
