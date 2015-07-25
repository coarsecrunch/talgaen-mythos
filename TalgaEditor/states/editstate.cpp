#include "editstate.h"
#include "glcontext.h"

#include <QApplication>
#include <QKeyEvent>
#include <QWheelEvent>

namespace talga
{
namespace editor
{
EditState::EditState(GLContext *context)
  :mContext(context)
  , mShift(false)
{
}

void EditState::keyPressEvent(QKeyEvent *e)
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
}

void EditState::keyReleaseEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Shift)
  {
    mShift = false;
    QApplication::restoreOverrideCursor();
  }

}
void EditState::wheelEvent(QWheelEvent *e)
{
  F32 amount = e->delta() / 400.0f;

  mContext->camera.box().setScaleX(mContext->camera.box().getScaleX() - amount);
  mContext->camera.box().setScaleY(mContext->camera.box().getScaleY() - amount);

  if (mContext->camera.box().getScaleX() <= 0.1f || mContext->camera.box().getScaleY() <= 0.1f)
  {
    mContext->camera.box().setScaleX(0.1f);
    mContext->camera.box().setScaleY(0.1f);
  }
  mContext->camera.update(0);


  mContext->update();
}

}
}
