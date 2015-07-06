#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <QPoint>
#include <QOpenGLWidget>
#include <vector>

#include "Renderer.h"
#include "Layer.h"
#include "Camera.h"
#include "Sprite.h"
#include "editormap.h"
#include "Rect.h"
#include "Point.h"

class QDragEnterEvent;
class QUndoCommand;

namespace talga
{
  namespace editor
  {
    typedef std::pair<std::string, std::vector<iPnt>> Selection;

    class GLContext : public QOpenGLWidget
    {
      Q_OBJECT
    public:
      GLContext(QWidget* parent);
      ~GLContext();

      virtual void initializeGL() override;
      virtual void paintGL() override;
      virtual void resizeGL(int w, int h) override;

      virtual void dragEnterEvent(QDragEnterEvent * e) override;
      virtual void dropEvent(QDropEvent *e) override;

      virtual void mousePressEvent(QMouseEvent* e) override;
      virtual void mouseReleaseEvent(QMouseEvent* e) override;
      virtual void mouseMoveEvent(QMouseEvent *e) override;
      virtual void wheelEvent(QWheelEvent* e) override;

      virtual void keyPressEvent(QKeyEvent* e) override;
      virtual void keyReleaseEvent(QKeyEvent* e) override;

    signals:
      void sig_addUndoCommand(QUndoCommand* c);
      void sig_updateHistoryMacro(bool startNewHistoryItem);
    public slots:
      void sl_addAsset(QString path);
      void sl_updateSelection(Selection);
      void sl_updateGL();

    protected:
      Camera camera;

      Layer mTileLayer;
      Layer mSpriteLayer;
      Layer mSelectionLayer;
      Renderer* mRenderer2D;
      AssetManager mManager;
      EditorMap mCurrentMap;

      //tracking
      Selection mCurrentSelection;
      QPoint mStartPos;
      vec3 mPreviousMousePos;
      bool mIsMouseDown;
      bool mShift;
      bool mStartNewHistoryItem;
      std::vector<Sprite> mSelectionRender;

    };

  }
}
#endif // GLCONTEXT_H
