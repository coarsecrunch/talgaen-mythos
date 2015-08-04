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

signals:
  void sig_assetChanged(const AAsset*, const std::string&);

public slots:
  void sl_assetSelected(AAsset*);
  void sl_updateChangedMap(EditorMap*);
private slots:
  void sl_itemDoubleClicked(QTreeWidgetItem* itm, int col);
  void sl_itemChanged(QTreeWidgetItem* itm, int col);
private:
  AAsset* mAsset;
  ASSET_TYPE mAssetType;

};

}
}
