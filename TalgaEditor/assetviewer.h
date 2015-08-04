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
    class AssetViewer : public QGraphicsView
    {
      Q_OBJECT

    public:
      AssetViewer(QWidget* );
      ~AssetViewer();

      virtual void mousePressEvent(QMouseEvent* e) override;
      virtual void mouseMoveEvent(QMouseEvent* e) override;
      virtual void mouseReleaseEvent(QMouseEvent* e) override;
    signals:
      void sig_updateSelection(Selection);

    public slots:
      void sl_updateTexture(cpAsset tex);
      void sl_updateChangedMap(EditorMap* map);
    protected:
      QGraphicsScene* pmImageViewScene;
      QGraphicsRectItem* mSelectBox;
      QPixmap* mCurrentImage;
      const AAsset* mCurrentAsset;

      bool mLeftButtonIsDown;
      QPoint mStartPos;
      QPoint mEndPos;
      QBrush mFill;
      QPen mLineProperties;
      void updateBox();
      EditorMap* mMap;
    };

  }
}
#endif // ASSETVIEWER_H
