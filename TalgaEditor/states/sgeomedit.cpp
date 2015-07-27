#include "sgeomedit.h"

#include <QMouseEvent>
#include <QApplication>
#include "glcontext.h"
#include "renderableshapes.h"

const int VERT_SIZE = 6;
const int MIN_VERT_DISTANCE = VERT_SIZE * 2;
namespace talga
{
namespace editor
{
SGeomEdit::SGeomEdit(GLContext* context)
  : EditState(context)
  , mIsMouseDown(false)
  , mSelectedGeom(nullptr)
  , mSelectedParent(nullptr)
  , isVertex(false)
{

}

void SGeomEdit::begin()
{
  mContext->mCurrentMap->setRenderSceneGeom(true);
  mContext->update();
}

void SGeomEdit::update(I32 dt)
{

}

void SGeomEdit::exit()
{
  mContext->mCurrentMap->setRenderSceneGeom(false);
  mContext->update();
}

void SGeomEdit::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    mIsMouseDown = true;
    mPreviousMousePos = vec3(e->x(), e->y(), 1.0f);

    mSelectedGeom = mContext->mCurrentMap->getSceneGeom(mContext->camera.screenToWorld(mPreviousMousePos));

    if (mSelectedGeom)
    {
      if (mSelectedGeom->getParent())
      {
        isVertex = true;

        if (mSelectedParent)
        {
          if (dynamic_cast<RdrRect*>(mSelectedParent))
          {
            ((RdrRect*)mSelectedParent)->setTransparency(0.5f);
          }
          else if (dynamic_cast<RdrTri*>(mSelectedParent))
          {
            ((RdrTri*)mSelectedParent)->setTransparency(0.3f);
          }
        }

        mSelectedParent = (IRenderable*)mSelectedGeom->getParent();

        if (mSelectedParent)
        {
          if (dynamic_cast<RdrRect*>(mSelectedParent))
          {
            ((RdrRect*)mSelectedParent)->setTransparency(0.9f);
          }
          else if (dynamic_cast<RdrTri*>(mSelectedParent))
          {
            ((RdrTri*)mSelectedParent)->setTransparency(0.7f);
          }
        }
      }
      else
      {
        if (mSelectedParent)
        {
          if (dynamic_cast<RdrRect*>(mSelectedParent))
          {
            ((RdrRect*)mSelectedParent)->setTransparency(0.5f);
          }
          else if (dynamic_cast<RdrTri*>(mSelectedParent))
          {
            ((RdrTri*)mSelectedParent)->setTransparency(0.3f);
          }
        }

        mSelectedParent = mSelectedGeom;

        if (mSelectedParent)
        {
          if (dynamic_cast<RdrRect*>(mSelectedParent))
          {
            ((RdrRect*)mSelectedParent)->setTransparency(0.9f);
          }
          else if (dynamic_cast<RdrTri*>(mSelectedParent))
          {
            ((RdrTri*)mSelectedParent)->setTransparency(0.7f);
          }
        }
      }
    }
    else
    {
      if (dynamic_cast<RdrRect*>(mSelectedParent))
      {
        ((RdrRect*)mSelectedParent)->setTransparency(0.5f);
      }
      else if (dynamic_cast<RdrTri*>(mSelectedParent))
      {
        ((RdrTri*)mSelectedParent)->setTransparency(0.3f);
      }
      mSelectedParent = nullptr;
      mContext->update();
    }

    mContext->update();
  }
}

void SGeomEdit::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Shift)
  {
    mShift = true;
    QApplication::setOverrideCursor(Qt::PointingHandCursor);
  }

  if ((e->key() == Qt::Key_Z)  && (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)))
  {
    emit mContext->sig_ctrlz();
  }

  if ((e->key() == Qt::Key_Y)  && (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)))
  {
    emit mContext->sig_ctrly();
  }

  if (e->key() == Qt::Key_Delete)
  {
    if (mSelectedParent)
    {
      mContext->mCurrentMap->removeSceneGeom(mSelectedParent);
      mSelectedParent = nullptr;
      mContext->update();
    }
  }
}

void SGeomEdit::keyReleaseEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Shift)
  {
    mShift = false;
    QApplication::restoreOverrideCursor();
  }

}

void SGeomEdit::mouseReleaseEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton)
  {
    mIsMouseDown = false;
    mSelectedGeom = nullptr;
    isVertex = false;
  }
}

static void updateParent(IRenderable* rect, std::vector<IRenderable*>& children)
{
  F32 x = (children[0]->getGlobalPosition().x() + children[1]->getGlobalPosition().x()) / 2.0f;
  F32 y = (children[0]->getGlobalPosition().y() + children[3]->getGlobalPosition().y()) / 2.0f;


  rect->box().setX(x);
  rect->box().setY(y);
  rect->box().setW(children[1]->getGlobalPosition().x() - children[0]->getGlobalPosition().x());
  rect->box().setH(children[3]->getGlobalPosition().y() - children[0]->getGlobalPosition().y());
  children[0]->box().setX(rect->box().getW() * -0.5f);
  children[0]->box().setY(rect->box().getH() * -0.5f);

  children[1]->box().setX(rect->box().getW() * 0.5f);
  children[1]->box().setY(rect->box().getH() * -0.5f);

  children[2]->box().setX(rect->box().getW() * 0.5f);
  children[2]->box().setY(rect->box().getH() * 0.5f);

  children[3]->box().setX(rect->box().getW() * -0.5f);
  children[3]->box().setY(rect->box().getH() * 0.5f);

}

void SGeomEdit::mouseMoveEvent(QMouseEvent *e)
{

  if (mIsMouseDown && !mShift && mContext->mCurrentMap)
  {
    mStartPos = e->pos();
    vec3 currentMousePos = vec3(e->x(), e->y(), 1.0f);
    vec3 dmouse = mContext->camera.screenToWorld(currentMousePos) - mContext->camera.screenToWorld(mPreviousMousePos);

    if (dynamic_cast<RdrRect*>(mSelectedGeom) && !isVertex)
    {
        RdrRect* rect = static_cast<RdrRect*>(mSelectedGeom);
        rect->box().setPosition(rect->box().getPosition() + dmouse);
        rect->box().updateVertsPosition();

        mContext->update();
    }
    else if (dynamic_cast<RdrTri*>(mSelectedGeom) && !isVertex)
    {
      RdrTri* tri = static_cast<RdrTri*>(mSelectedGeom);
      tri->getBase().setPosition(tri->getBase().getPosition() + dmouse);
      tri->getBase().updateVertsPosition();

      mContext->update();
    }
    else if (isVertex)
    {

      if (dynamic_cast<RdrRect*>((IRenderable*)mSelectedGeom->getParent()))
      {
        RdrRect* vertex = static_cast<RdrRect*>(mSelectedGeom);
        RdrRect* parent = const_cast<RdrRect*>( static_cast<const RdrRect*>(mSelectedGeom->getParent()));
        std::vector<IRenderable*> childrenNC;
        vec3 newpos = vertex->getGlobalPosition() + dmouse;

        for (auto i : parent->getChildren())
          childrenNC.push_back(const_cast<IRenderable*>(i));

        //TL
        if (vertex == parent->getChildren()[0])
        {
          if (newpos.y() >= parent->getChildren()[3]->getGlobalPosition().y() - MIN_VERT_DISTANCE || newpos.x() > parent->getChildren()[1]->getGlobalPosition().x() - MIN_VERT_DISTANCE)
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();

          childrenNC[1]->box().setY(vertex->box().getY());
          childrenNC[1]->box().updateVertsPosition();

          childrenNC[3]->box().setX(vertex->box().getX());
          childrenNC[3]->box().updateVertsPosition();
          //updateChildren(parent, childrenNC);
        } //TR
        else if (vertex == parent->getChildren()[1])
        {
          if (newpos.y() >= childrenNC[3]->getGlobalPosition().y() - MIN_VERT_DISTANCE || newpos.x() < childrenNC[0]->getGlobalPosition().x() + MIN_VERT_DISTANCE)
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();

          childrenNC[0]->box().setY(vertex->box().getY());
          childrenNC[0]->box().updateVertsPosition();

          childrenNC[2]->box().setX(vertex->box().getX());
          childrenNC[2]->box().updateVertsPosition();
        } //BR
        else if (vertex == parent->getChildren()[2])
        {
          if (newpos.y() <= childrenNC[1]->getGlobalPosition().y() + MIN_VERT_DISTANCE || newpos.x() < childrenNC[0]->getGlobalPosition().x() + MIN_VERT_DISTANCE)
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();

          childrenNC[3]->box().setY(vertex->box().getY());
          childrenNC[3]->box().updateVertsPosition();

          childrenNC[1]->box().setX(vertex->box().getX());
          childrenNC[1]->box().updateVertsPosition();
        }//BL
        else if (vertex == parent->getChildren()[3])
        {
          if (newpos.y() <= childrenNC[1]->getGlobalPosition().y() + MIN_VERT_DISTANCE || newpos.x() > parent->getChildren()[1]->getGlobalPosition().x() - MIN_VERT_DISTANCE)
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();

          childrenNC[2]->box().setY(vertex->box().getY());
          childrenNC[2]->box().updateVertsPosition();

          childrenNC[0]->box().setX(vertex->box().getX());
          childrenNC[0]->box().updateVertsPosition();
        }

        updateParent(parent, childrenNC);
      }

      if (dynamic_cast<RdrTri*>((IRenderable*)mSelectedGeom->getParent()))
      {
        RdrRect* vertex = static_cast<RdrRect*>(mSelectedGeom);
        RdrTri* parent = const_cast<RdrTri*>( static_cast<const RdrTri*>(mSelectedGeom->getParent()));
        std::vector<IRenderable*> childrenNC;
        vec3 newpos = vertex->getGlobalPosition() + dmouse;

        for (auto i : parent->getChildren())
          childrenNC.push_back(const_cast<IRenderable*>(i));

        if (vertex == childrenNC[0])
        {
          if (newpos.y() > childrenNC[1]->getGlobalPosition().y() - MIN_VERT_DISTANCE || newpos.y() > childrenNC[2]->getGlobalPosition().y() - MIN_VERT_DISTANCE )
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();
          parent->getBase().getRealVerts()[0] = vertex->box().getPosition() + dmouse;
          parent->getBase().updateVertsPosition();
        }
        else if (vertex == childrenNC[1])
        {
          if (newpos.y() < childrenNC[0]->getGlobalPosition().y() + MIN_VERT_DISTANCE || newpos.x() < childrenNC[2]->getGlobalPosition().x() + MIN_VERT_DISTANCE )
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();
          parent->getBase().getRealVerts()[1] = vertex->box().getPosition() + dmouse;
          parent->getBase().updateVertsPosition();
        }
        else if (vertex == childrenNC[2])
        {
          if (newpos.y() < childrenNC[0]->getGlobalPosition().y() + MIN_VERT_DISTANCE || newpos.x() > childrenNC[1]->getGlobalPosition().x() - MIN_VERT_DISTANCE )
            return;

          vertex->box().setPosition(vertex->box().getPosition() + dmouse);
          vertex->box().updateVertsPosition();
          parent->getBase().getRealVerts()[2] = vertex->box().getPosition() + dmouse;
          parent->getBase().updateVertsPosition();
        }
      }


      mContext->update();
    }

    mPreviousMousePos = currentMousePos;
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

  RdrRect* TL = new RdrRect(VERT_SIZE, VERT_SIZE, rect->box().getW() * -0.5f, rect->box().getH() * -0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
  RdrRect* TR = new RdrRect(VERT_SIZE, VERT_SIZE, rect->box().getW() * 0.5f, rect->box().getH() * -0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
  RdrRect* BR = new RdrRect(VERT_SIZE, VERT_SIZE, rect->box().getW() * 0.5f, rect->box().getH() * 0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));
  RdrRect* BL = new RdrRect(VERT_SIZE, VERT_SIZE, rect->box().getW() * -0.5f, rect->box().getH() * 0.5f, vec4(1.0f, 0.0f, 1.0f, 1.0f));

  rect->addChild(TL);
  rect->addChild(TR);
  rect->addChild(BR);
  rect->addChild(BL);



  mContext->mCurrentMap->addSceneGeom(rect);
  mContext->update();
}

void SGeomEdit::addTri(const vec3& pos)
{
  RdrTri* tri = new RdrTri( {vec2{-0.5f * mContext->mCurrentMap->getTileWidth(), -0.5f * mContext->mCurrentMap->getTileWidth()},
                             vec2{0.5f * mContext->mCurrentMap->getTileWidth(), 0.5f * mContext->mCurrentMap->getTileWidth()},
                             vec2{-0.5f * mContext->mCurrentMap->getTileWidth(),0.5f * mContext->mCurrentMap->getTileWidth()}},
                            pos.x(), pos.y(), vec4(0.0f, 1.0f, 0.0f, 1.0f) );

  RdrRect* T = new RdrRect(VERT_SIZE, VERT_SIZE, -0.5f * mContext->mCurrentMap->getTileWidth(), -0.5f * mContext->mCurrentMap->getTileWidth(), vec4(1.0f, 0.0f, 1.0f, 1.0f));
  RdrRect* BR = new RdrRect(VERT_SIZE, VERT_SIZE,0.5f * mContext->mCurrentMap->getTileWidth(), 0.5f * mContext->mCurrentMap->getTileWidth(), vec4(1.0f, 0.0f, 1.0f, 1.0f));
  RdrRect* BL = new RdrRect(VERT_SIZE, VERT_SIZE, -0.5f * mContext->mCurrentMap->getTileWidth(),0.5f * mContext->mCurrentMap->getTileWidth(), vec4(1.0f, 0.0f, 1.0f, 1.0f));

  tri->addChild(T);
  tri->addChild(BR);
  tri->addChild(BL);

  mContext->mCurrentMap->addSceneGeom(tri);
  mContext->update();
}

}
}
