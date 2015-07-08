#include "assetviewer.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QBrush>
#include "mainwindow.h"
#include <QDockWidget>
#include "Math/Operations.h"

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
      , mEndPos{-1, -1}
      , mFill(QColor(50, 50, 50, 150))
      , mLineProperties(Qt::NoPen)
      , mSelectBox{nullptr}
      , mLeftButtonIsDown{false}
    {
      setScene(pmImageViewScene);

      QColor color(50, 50, 50, 150);

      QBrush fillproperties(color);
      QPen lineProperties(Qt::NoPen);
    }

    AssetViewer::~AssetViewer()
    {
      pmImageViewScene->clear();
      delete pmImageViewScene;
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

        mLeftButtonIsDown = true;
      }
    }

    void AssetViewer::mouseMoveEvent(QMouseEvent *e)
    {
      if (mLeftButtonIsDown)
      {
        mEndPos = e->pos();
        updateBox();
      }
    }

    void AssetViewer::mouseReleaseEvent(QMouseEvent *e)
    {
      if (mStartPos.x() != -1 && e->button() == Qt::LeftButton)
      {
        QPoint currentPos = e->pos();
        QPoint mEndPos = e->pos();
        Selection s;

        s.first = mCurrentImage.first.toUtf8().constData();


        if (mStartPos.x() > currentPos.x())
        {
          if (mStartPos.y() > currentPos.y())
          {
            for (I32 y = currentPos.y() / TILE_HEIGHT; y <= mStartPos.y() / TILE_HEIGHT; ++y)
            {
              for (I32 x = currentPos.x() / TILE_WIDTH; x <= mStartPos.x() / TILE_WIDTH; ++x)
              {
                s.second.push_back(iPnt{x, y});
              }
            }
          }
          else
          {
            for (I32 y = mStartPos.y() / TILE_HEIGHT; y <= currentPos.y() / TILE_HEIGHT; ++y)
            {
              for (I32 x = currentPos.x() / TILE_WIDTH; x <= mStartPos.x() / TILE_WIDTH; ++x)
              {
                  s.second.push_back(iPnt{x, y});
              }
            }
          }
        }
        else
        {
          if (mStartPos.y() > currentPos.y())
          {
            for (I32 y = currentPos.y() / TILE_HEIGHT; y <= mStartPos.y() / TILE_HEIGHT; ++y)
            {
              for (I32 x = mStartPos.x() / TILE_WIDTH; x <= currentPos.x() / TILE_WIDTH; ++x)
              {
                s.second.push_back(iPnt{x, y});
              }
            }
          }
          else
          {
            for (I32 y = mStartPos.y() / TILE_HEIGHT; y <= currentPos.y() / TILE_HEIGHT; ++y)
            {
              for (I32 x = mStartPos.x() / TILE_WIDTH; x <= currentPos.x() / TILE_WIDTH; ++x)
              {
                s.second.push_back(iPnt{x, y});
              }
            }
          }
        }

        updateBox();
        mStartPos = mEndPos;
        mLeftButtonIsDown = false;

        emit sig_updateSelection(s);
      }

    }

    void AssetViewer::sl_updateTexture(TextureAsset pix)
    {
      pmImageViewScene->clear();


      static QPen lineProperties(Qt::gray);
      lineProperties.setWidth(1);

      mCurrentImage = pix;
      pmImageViewScene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(*pix.second)));

      int imageWidth = pix.second->width();
      int imageHeight = pix.second->height();

      for (int x = 0; x <= imageWidth / TILE_WIDTH; x++ )
      {
        //pmImageViewScene->addLine(x * TILE_WIDTH, 0, x * TILE_WIDTH, imageHeight, lineProperties);
      }

      for (int y = 0; y <= imageHeight / TILE_HEIGHT; y++)
      {
        //pmImageViewScene->addLine(0, y * TILE_HEIGHT, imageWidth, y * TILE_HEIGHT, lineProperties);
      }
    }

    void AssetViewer::sl_updateChangedMap(EditorMap* map)
    {
      pmImageViewScene->clear();
    }

    void AssetViewer::updateBox()
    {
      if (mSelectBox)
      {
        pmImageViewScene->removeItem(mSelectBox);
        delete mSelectBox;
      }


      int boxX = 0;
      int boxY = 0;
      int boxW = 0;
      int boxH = 0;

      if (mStartPos.x() < mEndPos.x())
      {
        boxX = (mStartPos.x() / TILE_WIDTH) * TILE_WIDTH;
        boxW = (((mEndPos.x() - boxX) / TILE_WIDTH) + 1)* TILE_WIDTH;
      }
      else
      {
        boxX = (mEndPos.x() / TILE_WIDTH) * TILE_WIDTH;
        boxW = (((mStartPos.x() - boxX) / TILE_WIDTH) + 1) * TILE_WIDTH;
      }

      if (mStartPos.y() < mEndPos.y())
      {
        boxY = (mStartPos.y() / TILE_HEIGHT) * TILE_HEIGHT;
        boxH = (((mEndPos.y() - boxY) / TILE_HEIGHT) + 1) * TILE_HEIGHT;
      }
      else
      {
        boxY = (mEndPos.y() / TILE_HEIGHT) * TILE_HEIGHT;
        boxH = (((mStartPos.y() - boxY) / TILE_HEIGHT) + 1) * TILE_HEIGHT;
      }
      if (boxX < 0)
        boxX = 0;
      if (boxY < 0)
        boxY = 0;

      mSelectBox = new QGraphicsRectItem(boxX, boxY, boxW, boxH);
      mSelectBox->setPen(mLineProperties);
      mSelectBox->setBrush(mFill);
      pmImageViewScene->addItem(mSelectBox);
    }

  }
}
