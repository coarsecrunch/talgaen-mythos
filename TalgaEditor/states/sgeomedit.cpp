#include "sgeomedit.h"

#include <QMouseEvent>

#include "glcontext.h"
#include "renderableshapes.h"

namespace talga
{
namespace editor
{
SGeomEdit::SGeomEdit(GLContext* context)
  : EditState(context)
  , mIsMouseDown(false)
{

}

void SGeomEdit::begin()
{
  mContext->mCurrentMap->setRenderSceneGeom(true);
}

void SGeomEdit::update(I32 dt)
{

}

void SGeomEdit::exit()
{
  mContext->mCurrentMap->setRenderSceneGeom(false);
}

void SGeomEdit::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    mIsMouseDown = true;
    mPreviousMousePos = vec3(e->x(), e->y(), 1.0f);
  }
}

void SGeomEdit::mouseReleaseEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    mIsMouseDown = false;
  }
}

void SGeomEdit::mouseMoveEvent(QMouseEvent *e)
{

  if (mIsMouseDown && !mShift && mContext->mCurrentMap && mContext->mCurrentMap->getWorkingLayer())
  {
    mStartPos = e->pos();
    vec3 pos = mContext->camera.screenToWorld(vec3{(F32)e->x(), (F32)e->y(), 1.0f});
  }
  else if (mIsMouseDown && mShift)
  {
    vec3 currentMousePos = vec3(e->x(), e->y(), 1.0f);
    vec3 dmouse = mContext->camera.screenToWorld(currentMousePos) - mContext->camera.screenToWorld(mPreviousMousePos);
    mContext->camera.box().setPosition(mContext->camera.box().getPosition() - dmouse);
    mContext->camera.update(0);
    mPreviousMousePos = currentMousePos;

    mContext->update();
  }

}

void SGeomEdit::addRect(const vec3 &pos)
{
  RdrRect* rect = new RdrRect(mContext->mCurrentMap->getTileWidth(),mContext->mCurrentMap->getTileHeight(), pos.x(), pos.y(), vec4(0.0f, 0.0f, 1.0f, 1.0f) );

  mContext->mCurrentMap->addSceneGeom(rect);
  mContext->update();
}

}
}
