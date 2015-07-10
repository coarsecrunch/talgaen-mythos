#pragma once

#include <QObject>
#include "Map.h"
namespace talga
{
  class AssetManager;
namespace editor
{
  class EditorMap;
class GData : public QObject
{
  Q_OBJECT
public:

  static GData* getInstance();
  const EditorMap* getCurrentMap();
  AssetManager* getManager();
  void destroy();
  void setCurrentMap(const Map& map);
public slots:
   void sl_saveMap();
   void sl_loadMap(const std::string& path);
   void sl_saveAs(const std::string& path);
signals:
  void sig_mapChanged(EditorMap* map);

private:
  EditorMap* mCurrentMap;
  AssetManager* mManager;

  GData();
};

}
}
