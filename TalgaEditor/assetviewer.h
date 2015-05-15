#ifndef ASSETVIEWER_H
#define ASSETVIEWER_H

#include <QGraphicsView>
#include <QPair>
#include "glcontext.h"

class QImage;
class QGraphicsScene;
class QGraphicsRectItem;

namespace talga
{
  namespace editor
  {  

      typedef QPair<QString, QImage*> TextureAsset;

    class AssetViewer : public QGraphicsView
    {
      Q_OBJECT

    public:
      AssetViewer(QWidget* );
      ~AssetViewer();

      virtual void mouseMoveEvent(QMouseEvent *event) override;
      virtual void mousePressEvent(QMouseEvent* e) override;
      virtual void mouseReleaseEvent(QMouseEvent* e) override;
    signals:
      void sig_updateSelection(Selection);

    public slots:
      void sl_updateTexture(TextureAsset);

    protected:
      QGraphicsScene* pmImageViewScene;
      QGraphicsRectItem* mSelectBox;
      TextureAsset mCurrentImage;
      void updateBox(int, int);

      QPoint mStartPos;
    };

  }
}
#endif // ASSETVIEWER_H
