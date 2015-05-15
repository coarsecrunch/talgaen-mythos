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


class QDragEnterEvent;

namespace talga
{
  namespace editor
  {
    typedef std::pair<std::string, Rect> Selection;

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
      //virtual void mouseMoveEvent(QMouseEvent *event) override;

    public slots:
      void sl_addAsset(QString path);
      void sl_updateSelection(Selection);

    protected:
      Camera camera;

      Layer mTileLayer;
      Layer mSpriteLayer;
      Renderer* mRenderer2D;
      AssetManager mManager;
      EditorMap mCurrentMap;
      Selection mCurrentSelection;
      QPoint mStartPos;
    };

  }
}
#endif // GLCONTEXT_H
