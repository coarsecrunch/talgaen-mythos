#include "gdata.h"
#include "editormap.h"
#include "AssetManager.h"
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

void GData::sl_saveMap()
{
  mCurrentMap->save("baldbelly.tmap", *mManager);
}

void GData::sl_loadMap()
{
  *mCurrentMap = EditorMap();
  mCurrentMap->load("baldbelly.tmap", *mManager);

  emit sig_mapChanged(mCurrentMap);
}


}
}
