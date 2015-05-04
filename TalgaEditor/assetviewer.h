#ifndef ASSETVIEWER_H
#define ASSETVIEWER_H

#include <QGraphicsView>

class QPixmap;
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

    virtual void mouseMoveEvent(QMouseEvent *event) override;


public slots:
    void sl_updateTexture(QPixmap*);

protected:
    QGraphicsScene* pmImageViewScene;
    QGraphicsRectItem* mSelectBox;

    void updateBox(int, int);
};

}
}
#endif // ASSETVIEWER_H
