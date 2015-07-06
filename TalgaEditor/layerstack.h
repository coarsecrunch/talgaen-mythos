#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <QTreeWidget>
#include <QObject>
#include <vector>
#include "editormap.h"
#include "Cmn.h"

namespace talga
{
namespace editor
{

class LayerStack : public QTreeWidget
{
public:
  LayerStack(QWidget* parent = nullptr);
  void mSetCurrentLayerIndex(I32 value) {mCurrentLayerIndex = value;}
signals:
  void sig_updateLayerPropertiesName(QString str);
  void sig_updateLayerPropertiesVisiblity(bool visible);
public slots:
  void sl_updateLayerStack(std::vector<MapLayer>* layers);
protected:
  std::vector<MapLayer>* mLayers;
  I32 mCurrentLayerIndex;
};

}
}

#endif // LAYERSTACK_H
