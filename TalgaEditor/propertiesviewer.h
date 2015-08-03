#pragma once

#include <QTreeWidget>
#include "AAsset.h"

namespace talga
{

namespace editor
{

class PropertiesViewer : public QTreeWidget
{
  Q_OBJECT
public:
  enum ASSET_TYPE {NONE, MAP};
  explicit PropertiesViewer(QWidget* parent = nullptr);

public slots:
  void sl_assetSelected(AAsset*);

private slots:
  void sl_itemDoubleClicked(QTreeWidgetItem* itm, int col);
private:
  AAsset* mAsset;
  ASSET_TYPE mAssetType;

};

}
}
