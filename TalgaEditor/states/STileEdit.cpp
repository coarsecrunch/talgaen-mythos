#include "STileEdit.h"
#include "glcontext.h"

#include <QMouseEvent>
#include <QDebug>
#include "commands/CInsertTiles.h"
#include "commands/cchangeworkinglayer.h"
#include "Sprite.h"
#include "gdata.h"
namespace talga
{
namespace editor
{
STileEdit::STileEdit(GLContext *context)
  : EditState(context)
  , mIsMouseDown{false}
  , mPreviousMousePos{0.0f, 0.0f, 0.0f}
  , mStartNewHistoryItem(true)
  , mCurrentSelection{"NULL", std::vector<iPnt>{}}
  , mMouseBeginClick{0.0f, 0.0f}
{

}

void STileEdit::begin()
{

}

void STileEdit::update(I32 dt)
{

}

void STileEdit::exit()
{
  mCurrentSelection = {"NULL", std::vector<iPnt>{}};
  mSelectionRender.clear();
  mContext->mSelectionLayer.clear();
  mContext->update();
}

void STileEdit::updateSelection(Selection selection)
{
  auto tex = GData::getInstance()->getManager()->GetTexture(selection.first);

  if (!tex) return;

  mContext->mCurrentMap->insertSheet(tex);
  mCurrentSelection = selection;

  auto tiles = mContext->mCurrentMap->getTiles(mCurrentSelection.second, GData::getInstance()->getManager()->GetTexture(mCurrentSelection.first));

  mContext->mSelectionLayer.clear();
  mSelectionRender.clear();
  mSelectionRender.reserve(tiles.size());
  for (auto& t : tiles)
  {
    mSelectionRender.push_back(Sprite{t.first, t.second, 0.5f, });
   mContext->mSelectionLayer.add(&mSelectionRender.back());
  }
}

void STileEdit::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    mIsMouseDown = true;
    mPreviousMousePos = vec3(e->x(), e->y(), 1.0f);
    mStartNewHistoryItem = true;
    mMouseBeginClick = vec3(e->x(), e->y(), 1.0f);
    if (!mShift && !(mCurrentSelection.first == "NULL") && mContext->mCurrentMap->getWorkingLayer())
    {
      mStartPos = e->pos();
      vec3 pos = mContext->camera.screenToWorld(vec3{(F32)e->x(), (F32)e->y(), 1.0f});

      if (pos(0) >= 0 && pos(0) < mContext->mCurrentMap->getTileWidth() * mContext->mCurrentMap->getWidth()
          && pos(1) >= 0 && pos(1) < mContext->mCurrentMap->getTileHeight() * mContext->mCurrentMap->getHeight())
      {
        std::vector<iPnt> tiles;

        //mCurrentMap->insertTile(mCurrentSelection.second, Rect{pos(0) / mCurrentMap->getTileWidth(), pos(1) / mCurrentMap->getTileHeight()}, mManager.GetTexture(mCurrentSelection.first));
        emit mContext->sig_updateHistoryMacro(mStartNewHistoryItem);
        emit mContext->sig_addUndoCommand(new CInsertTiles(mContext->mCurrentMap, mContext->mCurrentMap->getTiles(mCurrentSelection.second, GData::getInstance()->getManager()->GetTexture(mCurrentSelection.first)),
             iPnt(pos(0) / mContext->mCurrentMap->getTileWidth(), pos(1) / mContext->mCurrentMap->getTileHeight()),mCurrentSelection.second));

        mStartNewHistoryItem = false;
      }
    }

  }
}

void STileEdit::mouseReleaseEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    mIsMouseDown = false;
  }
  if (e->button() == Qt::LeftButton && mContext->mCurrentMap && mContext->mCurrentMap->getWorkingLayer())
  {
    emit mContext->sig_updateHistoryMacro(false);
    mStartNewHistoryItem = true;
  }


  if (mCtrl &&  !(mCurrentSelection.first == "NULL"))
  {
    qDebug() << "worked";

    vec3 endPos = vec3(e->x(), e->y(), 1.0f);

    mMouseBeginClick = mContext->camera.screenToWorld(mMouseBeginClick);
    endPos = mContext->camera.screenToWorld(endPos);

    I32 smallX = (mMouseBeginClick.x() < endPos.x()) ? mMouseBeginClick.x() : endPos.x();
    I32 smallY = (mMouseBeginClick.y() < endPos.y()) ? mMouseBeginClick.y() : endPos.y();
    I32 largeX = (mMouseBeginClick.x() > endPos.x()) ? mMouseBeginClick.x() : endPos.x();
    I32 largeY = (mMouseBeginClick.y() > endPos.y()) ? mMouseBeginClick.y() : endPos.y();

    smallX /=  mContext->mCurrentMap->getTileWidth();
    largeX /= mContext->mCurrentMap->getTileWidth();

    smallY /= mContext->mCurrentMap->getTileHeight();
    largeY /= mContext->mCurrentMap->getTileHeight();

    emit mContext->sig_updateHistoryMacro(true);
    for (I32 y = smallY; y <= largeY; ++y)
    {
      for (I32 x = smallX; x <= largeX; ++x)
      {
        emit mContext->sig_addUndoCommand(new CInsertTiles(mContext->mCurrentMap, mContext->mCurrentMap->getTiles(mCurrentSelection.second,
      GData::getInstance()->getManager()->GetTexture(mCurrentSelection.first)),
      iPnt(x, y),
      mCurrentSelection.second));
      }
    }
  emit mContext->sig_updateHistoryMacro(false);


    mContext->update();

  }

}

void STileEdit::mouseMoveEvent(QMouseEvent *e)
{
  if (mIsMouseDown && !mShift && mContext->mCurrentMap && mContext->mCurrentMap->getWorkingLayer() && !mCtrl)
  {
    mStartPos = e->pos();
    vec3 pos = mContext->camera.screenToWorld(vec3{(F32)e->x(), (F32)e->y(), 1.0f});

    if (pos(0) >= 0 && pos(0) < mContext->mCurrentMap->getTileWidth() * mContext->mCurrentMap->getWidth()
        && pos(1) >= 0 && pos(1) < mContext->mCurrentMap->getTileHeight() * mContext->mCurrentMap->getHeight())
    {
      if ( !(mCurrentSelection.first == "NULL") )
      {
        if (mStartNewHistoryItem)
        {
          emit mContext->sig_updateHistoryMacro(mStartNewHistoryItem);
          mStartNewHistoryItem = false;
        }

        emit mContext->sig_addUndoCommand(new CInsertTiles(mContext->mCurrentMap, mContext->mCurrentMap->getTiles(mCurrentSelection.second,
      GData::getInstance()->getManager()->GetTexture(mCurrentSelection.first)),
      iPnt(pos(0) / mContext->mCurrentMap->getTileWidth(), pos(1) / mContext->mCurrentMap->getTileHeight()),
      mCurrentSelection.second));

      }

      mContext->update();
    }
  }
  else if (mIsMouseDown && mShift && !mCtrl)
  {
    vec3 currentMousePos = vec3(e->x(), e->y(), 1.0f);
    vec3 dmouse = mContext->camera.screenToWorld(currentMousePos) - mContext->camera.screenToWorld(mPreviousMousePos);
    mContext->camera.box().setPosition(mContext->camera.box().getPosition() - dmouse);
    mContext->camera.update(0);
    mPreviousMousePos = currentMousePos;

    mContext->update();
  }

  // show transparent selection under mouse
  if ( !(mCurrentSelection.first == "NULL"))
  {
    I32 tW = mContext->mCurrentMap->getTileWidth();
    I32 tH = mContext->mCurrentMap->getTileHeight();

    if (mSelectionRender.size() > 0)
    {
      I32 greatestX = 0;
      I32 greatestY = 0;
      I32 smallestX = 100000;
      I32 smallestY = 100000;
      I32 greatestXIndex = 0;
      I32 greatestYIndex = 0;
      I32 smallestXIndex = 0;
      I32 smallestYIndex = 0;
      I32 count = 0;

      for (const auto& pnt : mCurrentSelection.second)
      {
        if (pnt.x() > greatestX)
        {
          greatestX = pnt.x();
          greatestXIndex = count;
        }

        if (pnt.y() > greatestY)
        {
          greatestY = pnt.y();
          greatestYIndex = count;
        }

        if (pnt.x() < smallestX)
        {
          smallestX = pnt.x();
          smallestXIndex = pnt.x();
        }

        if (pnt.y() < smallestY)
        {
          smallestY = pnt.y();
          smallestYIndex = pnt.y();
        }

        ++count;
      }

      for (const auto& pnt : mCurrentSelection.second)
      {
        vec3 setTo(e->x(), e->y(), 1.0f);
        setTo = mContext->camera.screenToWorld(setTo);
        setTo[0] -= (I32)setTo(0) % tW - (0.5f * tW);
        setTo[1] -= (I32)setTo(1) % tH - (0.5f * tH);
        setTo[0] += (pnt.x() - smallestX) * tW;
        setTo[1] += (pnt.y() - smallestY) * tH;

        setTo[0] = (I32)setTo(0);
        setTo[1] = (I32)setTo(1);

        mSelectionRender[ (pnt.y() - smallestY) * (greatestX - smallestX + 1) + (pnt.x() - smallestX)].box().setPosition(setTo);
        mSelectionRender[ (pnt.y() - smallestY) * (greatestX - smallestX + 1) + (pnt.x() - smallestX)].box().updateVertsPosition();
      }

      mContext->update();
    }
  }

}

}\
}
