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
    AssetViewer::AssetViewer(QWidget* parent)
      : QGraphicsView(parent)
      , pmImageViewScene(new QGraphicsScene())
      , mCurrentImage{"", nullptr}
      , mStartPos{-1,-1}
      , mEndPos{-1, -1}
      , mFill(QColor(50, 50, 50, 150))
      , mLineProperties(Qt::NoPen)
      , mSelectBox{nullptr}
      , mLeftButtonIsDown{false}
      , mMap{nullptr}
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
            for (I32 y = currentPos.y() / mMap->getTileHeight(); y <= mStartPos.y() / mMap->getTileHeight(); ++y)
            {
              for (I32 x = currentPos.x() / mMap->getTileWidth(); x <= mStartPos.x() / mMap->getTileWidth(); ++x)
              {
                s.second.push_back(iPnt{x, y});
              }
            }
          }
          else
          {
            for (I32 y = mStartPos.y() / mMap->getTileHeight(); y <= currentPos.y() / mMap->getTileHeight(); ++y)
            {
              for (I32 x = currentPos.x() / mMap->getTileWidth(); x <= mStartPos.x() / mMap->getTileWidth(); ++x)
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
            for (I32 y = currentPos.y() / mMap->getTileHeight(); y <= mStartPos.y() / mMap->getTileHeight(); ++y)
            {
              for (I32 x = mStartPos.x() / mMap->getTileWidth(); x <= currentPos.x() / mMap->getTileWidth(); ++x)
              {
                s.second.push_back(iPnt{x, y});
              }
            }
          }
          else
          {
            for (I32 y = mStartPos.y() / mMap->getTileHeight(); y <= currentPos.y() / mMap->getTileHeight(); ++y)
            {
              for (I32 x = mStartPos.x() / mMap->getTileWidth(); x <= currentPos.x() / mMap->getTileWidth(); ++x)
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
      if (!mMap) return;
      pmImageViewScene->clear();


      static QPen lineProperties(Qt::gray);
      lineProperties.setWidth(1);

      mCurrentImage = pix;
      pmImageViewScene->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(*pix.second)));

      int imageWidth = pix.second->width();
      int imageHeight = pix.second->height();

      for (int x = 0; x <= imageWidth / mMap->getTileWidth(); x++ )
      {
        //pmImageViewScene->addLine(x * mMap->getTileWidth(), 0, x * mMap->getTileWidth(), imageHeight, lineProperties);
      }

      for (int y = 0; y <= imageHeight / mMap->getTileHeight(); y++)
      {
        //pmImageViewScene->addLine(0, y * mMap->getTileHeight(), imageWidth, y * mMap->getTileHeight(), lineProperties);
      }
    }

    void AssetViewer::sl_updateChangedMap(EditorMap* map)
    {
      pmImageViewScene->clear();
      mMap = map;
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
        boxX = (mStartPos.x() / mMap->getTileWidth()) * mMap->getTileWidth();
        boxW = (((mEndPos.x() - boxX) / mMap->getTileWidth()) + 1)* mMap->getTileWidth();
      }
      else
      {
        boxX = (mEndPos.x() / mMap->getTileWidth()) * mMap->getTileWidth();
        boxW = (((mStartPos.x() - boxX) / mMap->getTileWidth()) + 1) * mMap->getTileWidth();
      }

      if (mStartPos.y() < mEndPos.y())
      {
        boxY = (mStartPos.y() / mMap->getTileHeight()) * mMap->getTileHeight();
        boxH = (((mEndPos.y() - boxY) / mMap->getTileHeight()) + 1) * mMap->getTileHeight();
      }
      else
      {
        boxY = (mEndPos.y() / mMap->getTileHeight()) * mMap->getTileHeight();
        boxH = (((mStartPos.y() - boxY) / mMap->getTileHeight()) + 1) * mMap->getTileHeight();
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
