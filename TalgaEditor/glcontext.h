#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <QPoint>
#include <QOpenGLWidget>
#include <vector>
#include <memory>

#include "Renderer.h"
#include "Layer.h"
#include "Camera.h"
#include "Sprite.h"
#include "editormap.h"
#include "Rect.h"
#include "Point.h"
#include "StateMachine.h"

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
      void sig_loadAsset(QString path);
      void sig_updateLayerStack(EditorMap* newLayers);
      void sig_ctrlz();
      void sig_ctrly();
    public slots:
      void sl_addAsset(QString path);
      void sl_updateSelection(Selection);
      void sl_updateGL();
      void sl_updateChangedMap(EditorMap* newMap);
      void sl_setModeCollisionEdit();
      void sl_setModeTileEdit();
      void sl_addRect();
      void sl_addTri();
    protected:
      friend class EditState;
      friend class SGeomEdit;
      friend class STileEdit;
      Camera camera;
      Layer mSpriteLayer;
      Layer mTileLayer;
      Layer mSelectionLayer;
      std::shared_ptr<Renderer> mRenderer2D;
      EditorMap* mCurrentMap;


      StateMachine mState;
      //tracking
    };

  }
}
#endif // GLCONTEXT_H
