#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <QTreeWidget>
#include <QObject>
#include <vector>
#include "editormap.h"
#include "Cmn.h"
#include <QUndoCommand>
namespace talga
{
namespace editor
{

class LayerStack : public QTreeWidget
{
  Q_OBJECT
public:
  LayerStack(QWidget* parent = nullptr);
  void mSetCurrentLayerIndex(I32 value) {mCurrentLayerIndex = value;}
signals:
  void sig_addUndoCommand(QUndoCommand* cmd);
  void sig_updateGL();
public slots:
  void sl_updateLayerStack(EditorMap* map);
  void sl_updateItemChanged(QTreeWidgetItem* itm, int column);
private slots:
  void sl_changeWorkingLayer(QTreeWidgetItem* itm, int column);
private:
  EditorMap* mMap;
  I32 mCurrentLayerIndex;

};

}
}

#endif // LAYERSTACK_H
