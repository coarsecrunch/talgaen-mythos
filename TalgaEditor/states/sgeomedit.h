#pragma once

#include "editstate.h"
#include "Math/Vector3.h"
#include "IRenderable.h"

#include <QPoint>

namespace talga
{
namespace editor
{
  class GLContext;

  class SGeomEdit : public EditState
  {
  public:
    SGeomEdit(GLContext* context);

    virtual void begin() override;
    virtual void update(I32 dt) override;
    virtual void exit() override;

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void keyPressEvent(QKeyEvent* e) override;
    virtual void keyReleaseEvent(QKeyEvent *e) override;

    void addRect(const vec3& pos);
    void addTri(const vec3& pos);
  protected:

    bool mIsMouseDown;
    vec3 mPreviousMousePos;
    QPoint mStartPos;
    IRenderable* mSelectedGeom;
    IRenderable* mSelectedParent;
    bool isVertex;
  };
}
}
