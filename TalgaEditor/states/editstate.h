#pragma once

#include "IState.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

namespace talga
{
namespace editor
{
  class GLContext;
class EditState : public IState
{
public:
  EditState(GLContext* context);
  virtual void mousePressEvent(QMouseEvent* e) = 0;
  virtual void mouseReleaseEvent(QMouseEvent* e) = 0;
  virtual void mouseMoveEvent(QMouseEvent *e) = 0;
  virtual void wheelEvent(QWheelEvent* e);
  virtual void keyPressEvent(QKeyEvent* e);
  virtual void keyReleaseEvent(QKeyEvent* e);

protected:
  GLContext* mContext;

  bool mShift;
};
}
}
