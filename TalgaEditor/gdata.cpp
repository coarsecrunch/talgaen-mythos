#include "gdata.h"
#include "editormap.h"
#include "AssetManager.h"

#include <QFileDialog>
#include <QString>

namespace talga
{
namespace editor
{
GData::GData()
 : mManager(new AssetManager())
 , mCurrentMap(new EditorMap())
{
}

GData* GData::getInstance()
{
  static GData* data = new GData();
  return data;
}

EditorMap *GData::getCurrentMap()
{
  return mCurrentMap;
}

AssetManager *GData::getManager()
{
  return mManager;
}

void GData::destroy()
{
  delete mManager;
  delete mCurrentMap;
}

void GData::sl_saveMap(std::string path)
{
  mCurrentMap->save(path, *mManager);
}

void GData::sl_loadMap(std::string path)
{
  *mCurrentMap = EditorMap();
  mCurrentMap->load(path, *mManager);

  emit sig_mapChanged(mCurrentMap);
}


}
}
