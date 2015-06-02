#include "assetviewer.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include "mainwindow.h"
#include <QDockWidget>

namespace talga
{
  namespace editor
  {

    const int TILE_WIDTH = 32;
    const int TILE_HEIGHT = 32;

    AssetViewer::AssetViewer(QWidget* parent)
      : QGraphicsView(parent)
      , pmImageViewScene(new QGraphicsScene)
      , mCurrentImage{"", nullptr}
      , mStartPos{-1,-1}
    {
      setScene(pmImageViewScene);

      QColor color(50, 50, 50, 150);

      QBrush fillproperties(color);
      QPen lineProperties(Qt::NoPen);

      mSelectBox = new QGraphicsRectItem( 0, 0, 0 + TILE_WIDTH, 0 + TILE_HEIGHT);
      mSelectBox->setBrush(fillproperties);
      mSelectBox->setPen(Qt::NoPen);

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

    void AssetViewer::mousePressEvent(QMouseEvent *e)
    {
      if (e->button() == Qt::LeftButton)
      {
        if (!mCurrentImage.second) return;


        if (e->x() >= 0 && e->x() < mCurrentImage.second->width() && e->y() >= 0 && e->y() < mCurrentImage.second->height())
        {
          mStartPos = e->pos();
        }
      }
    }

    void AssetViewer::mouseReleaseEvent(QMouseEvent *e)
    {
      if (mStartPos.x() != -1 && e->button() == Qt::LeftButton)
      {
        QPoint currentPos = e->pos();

        Selection s;

        s.first = mCurrentImage.first.toUtf8().constData();


        if (mStartPos.x() > currentPos.x())
        {
          if (mStartPos.y() > currentPos.y())
          {
            for (I32 y = currentPos.y(); y < mStartPos.y(); y += TILE_HEIGHT)
            {
              for (I32 x = currentPos.x(); x < mStartPos.x(); x += TILE_WIDTH)
              {
                s.second.push_back(Point(x, y));
              }
            }
/*
            s.second.x = currentPos.x() / TILE_WIDTH;
            s.second.w = (mStartPos.x() / TILE_WIDTH) - (currentPos.x() / TILE_WIDTH) + 1;
            s.second.y = currentPos.y() / TILE_HEIGHT;
            s.second.h = (mStartPos.y() / TILE_HEIGHT) - (currentPos.y() / TILE_HEIGHT) + 1;
  */        }
          else
          {
            for (I32 y = mStartPos.y(); y < currentPos.y(); y += TILE_HEIGHT)
            {
              for (I32 x = currentPos.x(); x < mStartPos.x(); x += TILE_WIDTH)
              {
                s.second.push_back(Point(x, y));
              }
            }
/*
            s.second.x = currentPos.x() / TILE_WIDTH;
            s.second.w = (mStartPos.x() / TILE_WIDTH) - (currentPos.x() / TILE_WIDTH) + 1;
            s.second.y = mStartPos.y() / TILE_HEIGHT;
            s.second.h = (currentPos.y() / TILE_HEIGHT) - (mStartPos.y() / TILE_HEIGHT) + 1;
*/          }
        }
        else
        {
          if (mStartPos.y() > currentPos.y())
          {
            for (I32 y = currentPos.y(); y < mStartPos.y(); y += TILE_HEIGHT)
            {
              for (I32 x = mStartPos.x(); x < currentPos.x(); x += TILE_WIDTH)
              {
                s.second.push_back(Point(x, y));
              }
            }
/*
            s.second.x = mStartPos.x() / TILE_WIDTH;
            s.second.w = ( (currentPos.x() / TILE_WIDTH ) - (mStartPos.x() / TILE_WIDTH) )  + 1;
            s.second.y = currentPos.y() / TILE_HEIGHT;
            s.second.h = (mStartPos.y() / TILE_HEIGHT) - (currentPos.y() / TILE_HEIGHT) + 1;
*/
          }
          else
          {
            for (I32 y = mStartPos.y() / TILE_HEIGHT; y <= currentPos.y() / TILE_HEIGHT; ++y)
            {
              for (I32 x = mStartPos.x() / TILE_WIDTH; x <= currentPos.x() / TILE_WIDTH; ++x)
              {
                s.second.push_back(Point(x, y));
              }
            }

            /*
            s.second.x = mStartPos.x() / TILE_WIDTH;
            s.second.w = (currentPos.x() / TILE_WIDTH) - (mStartPos.x() / TILE_WIDTH) + 1;
            s.second.y = mStartPos.y() / TILE_HEIGHT;
            s.second.h = (currentPos.y() / TILE_HEIGHT) - (mStartPos.y() / TILE_HEIGHT) + 1;
            */
}
        }

        mStartPos.setX(-1);
        mStartPos.setY(-1);

        emit sig_updateSelection(s);
      }

    }

    void AssetViewer::sl_updateTexture(TextureAsset pix)
    {
      pmImageViewScene->removeItem(mSelectBox);
      pmImageViewScene->clear();


      static QPen lineProperties(Qt::gray);
      lineProperties.setWidth(1);

      mCurrentImage = pix;
      pmImageViewScene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(*pix.second)));

      int imageWidth = pix.second->width();
      int imageHeight = pix.second->height();

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
