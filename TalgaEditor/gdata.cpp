#include "gdata.h"
#include "editormap.h"
#include "AssetManager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

namespace talga
{
namespace editor
{
GData::GData()
 : mManager(new AssetManager())
 , mCurrentMap(new EditorMap())
 , mHasMap{false}
{
}

GData* GData::getInstance()
{
  static GData* data = new GData();
  return data;
}

const EditorMap *GData::getCurrentMap()
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

void GData::setCurrentMap(const Map& map)
{
  mHasMap = true;
  *mCurrentMap = map;
  emit sig_mapChanged(mCurrentMap);
}

void GData::sl_saveMap()
{
  if (mCurrentMap)
  {
    mCurrentMap->save(mCurrentMap->getPath() + mCurrentMap->getName(), *mManager);
    emit sig_mapChanged(mCurrentMap);
  }
}

void GData::sl_saveAs(const std::string& path)
{
  if (mCurrentMap)
  {
    mCurrentMap->save(path, *mManager);
  }
}

void GData::sl_loadMap(const std::string& path)
{
  if(mCurrentMap->load(path, *mManager))
  {
    mHasMap = true;
    emit sig_mapChanged(mCurrentMap);
  }
  else
  {
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString("map " + path + " is corrupted, or does not exist"));
    msgBox.exec();

    emit sig_mapChanged(nullptr);
  }


}


}
}
