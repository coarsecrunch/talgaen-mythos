#pragma once

#include <vector>

#include "editstate.h"
#include <QPoint>
#include "Math/Vector3.h"
#include "Sprite.h"
#include "Point.h"

namespace talga
{
  namespace editor
  {
  typedef std::pair<std::string, std::vector<iPnt>> Selection;
  class GLContext;

  class STileEdit : public EditState
  {
public:
    STileEdit(GLContext* context);
    virtual void begin() override;
    virtual void update(I32 dt) override;
    virtual void exit() override;

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    void updateSelection(Selection selection);
  private:
    vec3 mMouseBeginClick;
    QPoint mStartPos;
    vec3 mPreviousMousePos;
    bool mIsMouseDown;
    bool mStartNewHistoryItem;
    std::vector<Sprite> mSelectionRender;
    Selection mCurrentSelection;

  };

  }
}
