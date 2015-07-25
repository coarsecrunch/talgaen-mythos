#include "GL/glew.h"

#include "glcontext.h"

#include <vector>
#include <memory>

#include <QSurfaceFormat>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QApplication>
#include <QMimeData>


#include "Texture.h"
#include "IRenderable.h"

#include "states/STileEdit.h"
#include "states/sgeomedit.h"
#include "gdata.h"


namespace talga
{
  namespace editor
  {

    GLContext::GLContext(QWidget* parent)
      : QOpenGLWidget(parent)
      , camera{width(), height()}
      , mRenderer2D{nullptr}
      , mTileLayer{nullptr, width(), height()}
      , mSelectionLayer{nullptr, width(), height()}
      , mSpriteLayer{nullptr, width(), height()}
      , mCurrentMap{nullptr}

      , mState()
    {
      QSurfaceFormat format;
      format.setVersion(3, 3);
      format.setProfile(QSurfaceFormat::CoreProfile);
      setFormat(format);
      blockSignals(true);
    }

    void GLContext::initializeGL()
    {
      glewExperimental = true;

      GLenum err = glewInit();
      TALGA_ASSERT(err == GLEW_OK, "Failed to initialize glew, exiting...");

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      mRenderer2D = std::shared_ptr<Renderer>(new Renderer("../assets/shaders/renderer2d.vert", "../assets/shaders/renderer2d.frag"));

      mTileLayer.setRenderer(mRenderer2D);
      mTileLayer.setProjectionMatrix(width(), height());
      mSelectionLayer.setRenderer(mRenderer2D);
      mSelectionLayer.setProjectionMatrix(width(), height());
      mSpriteLayer.setRenderer(mRenderer2D);
      mSpriteLayer.setProjectionMatrix(width(), height());
      camera.box().setW(width());
      camera.box().setH(height());
      camera.update(0);

      mRenderer2D->setCamera(&camera);

      mTileLayer.add(mCurrentMap);

      camera.box().setX(100);
      camera.box().setY(200);

       GData::getInstance()->sl_loadMap("../assets/maps/sandboxx.tmap");
    }

    void GLContext::dragEnterEvent(QDragEnterEvent *e)
    {
      e->acceptProposedAction();
      QOpenGLWidget::dragEnterEvent(e);
    }

    void GLContext::dropEvent(QDropEvent *e)
    {
      QString qassetName = e->mimeData()->text();

      std::string assetname = qassetName.toUtf8().constData();

      QString fileEnding = qassetName.split(".").at(0);

      if ( fileEnding == "png" )
      {
        mSpriteLayer.add(new Sprite(GData::getInstance()->getManager()->GetTexture(assetname)));
        update();
      }
      else if (fileEnding == "tmap")
        qDebug() << "implement map drag motha belly";
      else
        TALGA_WARN(0, "tried to pass invalid file type to GLContext, file not loaded\nmust be a .tmap or .png");
    }

    void GLContext::sl_addAsset(QString path)
    {
      QString fileEnding = path.split(".").at(path.split(".").size() - 1);
      QString relPathQstr = QDir(QDir::currentPath()).relativeFilePath(path);
      std::string relPath = relPathQstr.toUtf8().constData();

#ifdef TALGA_WINDOWS_BUILD
      QString fileName = path.split("\\").at(path.split("\\").size() - 1);
#endif

      TALGA_MSG(std::string("file ending: ") + fileEnding.toStdString());

      if ( fileEnding == "png" )
      {
        GData::getInstance()->getManager()->AddTexture(path.toStdString());
      }

      else if (fileEnding == "tmap")
      {
        GData::getInstance()->getManager()->AddMap(path.toStdString());
      }

      else
        TALGA_WARN(0, "tried to pass invalid file type to GLContext, file not loaded\nmust be a .tmap or .png");
    }

    void GLContext::sl_updateSelection(Selection selection)
    {
      if (dynamic_cast<STileEdit*>(mState.getCurrentState()))
      {
        static_cast<STileEdit*>(mState.getCurrentState())->updateSelection(selection);
      }
    } 

    void GLContext::sl_updateGL()
    {
      if (mCurrentMap)
      {
        update();
      }
    }

    void GLContext::sl_updateChangedMap(EditorMap *newMap)
    {
      mTileLayer.clear();
      mSpriteLayer.clear();
      mSelectionLayer.clear();

      mCurrentMap = newMap;

      if (newMap)
      {
        mTileLayer.add(mCurrentMap);
      }

      sl_setModeTileEdit();
      blockSignals(false);
      sl_updateGL();
    }

    void GLContext::sl_setModeCollisionEdit()
    {
      if(mCurrentMap)
        mState.changeState(new SGeomEdit(this));
    }

    void GLContext::sl_setModeTileEdit()
    {
      if (mCurrentMap)
        mState.changeState(new STileEdit(this));
    }

    void GLContext::sl_addRect()
    {
      if (dynamic_cast<SGeomEdit*>(mState.getCurrentState()))
        static_cast<SGeomEdit*>(mState.getCurrentState())->addRect(camera.box().getPosition());
    }

    void GLContext::sl_addTri()
    {

    }

    void GLContext::paintGL()
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      mTileLayer.getRenderer()->tStackPush(camera.getCameraMat());
      mTileLayer.render();
      mSpriteLayer.render();
      mSelectionLayer.render();
      mSelectionLayer.getRenderer()->tStackPop();
    }

    void GLContext::resizeGL(int w, int h)
    {
      mTileLayer.setProjectionMatrix(w, h);
      mSpriteLayer.setProjectionMatrix(w, h);
      camera.setW(w);
      camera.setH(h);
      glViewport(0,0,w,h);
    }

    void GLContext::mousePressEvent(QMouseEvent *e)
    {
      if (mState.getCurrentState())
        static_cast<EditState*>(mState.getCurrentState())->mousePressEvent(e);
    }

    void GLContext::mouseMoveEvent(QMouseEvent *e)
    {
      if (mState.getCurrentState())
        static_cast<EditState*>(mState.getCurrentState())->mouseMoveEvent(e);
    }

    void GLContext::mouseReleaseEvent(QMouseEvent *e)
    {
      if (mState.getCurrentState())
        static_cast<EditState*>(mState.getCurrentState())->mouseReleaseEvent(e);
    }

    void GLContext::wheelEvent(QWheelEvent *e)
    {
      if (mState.getCurrentState())
        static_cast<EditState*>(mState.getCurrentState())->wheelEvent(e);
    }

    void GLContext::keyPressEvent(QKeyEvent *e)
    {
      if (mState.getCurrentState())
        static_cast<EditState*>(mState.getCurrentState())->keyPressEvent(e);
    }

    void GLContext::keyReleaseEvent(QKeyEvent *e)
    {
      if (mState.getCurrentState())
        static_cast<EditState*>(mState.getCurrentState())->keyReleaseEvent(e);

    }

    GLContext::~GLContext()
    {
      mSelectionLayer.clear();
    }

  }
}
