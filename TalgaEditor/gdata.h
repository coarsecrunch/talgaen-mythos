#pragma once

#include <QObject>

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
  EditorMap* getCurrentMap();
  AssetManager* getManager();
  void destroy();

public slots:
   void sl_saveMap();
   void sl_loadMap();

signals:
  void sig_mapChanged(EditorMap* map);

private:
  EditorMap* mCurrentMap;
  AssetManager* mManager;

  GData();
};

}
}
