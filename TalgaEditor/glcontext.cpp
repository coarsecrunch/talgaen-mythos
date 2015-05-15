#include "GL/glew.h"

#include "glcontext.h"
#include <QSurfaceFormat>
#include "Sprite.h"
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QMimeData>

namespace talga
{
  namespace editor
  {

    int WIDTH = 800, HEIGHT = 600;

    GLContext::GLContext(QWidget* parent)
      : QOpenGLWidget(parent)
      , camera{width(), height()}
      , mRenderer2D{nullptr}
      , mTileLayer{nullptr, width(), height()}
      , mSpriteLayer{nullptr, width(), height()}
      , mManager{}
      , mCurrentMap()
      , mCurrentSelection{"NULL", Rect{-1,-1,-1,-1}}
    {
      QSurfaceFormat format;
      format.setVersion(3, 3);
      format.setProfile(QSurfaceFormat::CoreProfile);
    }

    void GLContext::initializeGL()
    {
      glewExperimental = true;

      GLenum err = glewInit();
      TALGA_ASSERT(err == GLEW_OK, "Failed to initialize glew, exiting...");

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      mRenderer2D = new Renderer("../../../assets/shaders/renderer2d.vert", "../../../assets/shaders/renderer2d.frag");

      mSpriteLayer.setRenderer(mRenderer2D);
      mSpriteLayer.setProjectionMatrix(width(), height());
      mTileLayer.setRenderer(mRenderer2D);
      mTileLayer.setProjectionMatrix(width(), height());

      camera.getBox().setW(width());
      camera.getBox().setH(height());
      camera.update(0);

      mRenderer2D->setCamera(&camera);

      mManager.AddTexture("../../../assets/sprite_sheet.png");
      mCurrentMap = *mManager.AddMap("../../../assets/test.map");

      mCurrentMap.insertSheet(mManager.GetTexture("sprite_sheet.png"));

      mTileLayer.add(&mCurrentMap);

      camera.getBox().setX(100);
      camera.getBox().setY(200);
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

      QString fileEnding = qassetName.split(".").at(qassetName.split(".").size() - 1);

      if ( fileEnding == "png" )
      {
        mSpriteLayer.add(new Sprite(mManager.GetTexture(assetname)));
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
      QString fileName = path.split("/").at(path.split("/").size() - 1);

      if ( fileEnding == "png" )
      {
        mCurrentMap.insertSheet(mManager.AddTexture(relPath));
      }

      else if (fileEnding == "tmap")
      {
        mManager.AddMap(relPath);
      }

      else
        TALGA_WARN(0, "tried to pass invalid file type to GLContext, file not loaded\nmust be a .tmap or .png");
    }

    void GLContext::sl_updateSelection(Selection selection)
    {
      auto tex = mManager.GetTexture(selection.first);

      if (!tex) return;

      mCurrentMap.insertSheet(tex);
      mCurrentSelection = selection;

      std::cout << selection.first << "\tx:" << selection.second.x << "\ty:" << selection.second.y
                << "\tw:" << selection.second.w << "\th:" << selection.second.h << std::endl;

    }

    void GLContext::paintGL()
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      mTileLayer.getRenderer()->tStackPush(camera.getCameraMat());
      mTileLayer.render();
      mTileLayer.getRenderer()->tStackPop();

      mSpriteLayer.getRenderer()->tStackPush(camera.getCameraMat());
      mSpriteLayer.render();
      mSpriteLayer.getRenderer()->tStackPop();
    }

    void GLContext::resizeGL(int w, int h)
    {
      mSpriteLayer.setProjectionMatrix(w, h);
      mTileLayer.setProjectionMatrix(w, h);
      camera.setW(w);
      camera.setH(h);
      glViewport(0,0,w,h);
    }

    void GLContext::mousePressEvent(QMouseEvent *e)
    {
      mStartPos = e->pos();
      vec3 pos = camera.screenToWorld(vec3{(F32)e->x(), (F32)e->y(), 1.0f});
      qDebug() << "x: " << pos[0] << "\ty: " << pos[1];

      if ( !(mCurrentSelection.first == "NULL") )
      {
        mCurrentMap.insertTile(mCurrentSelection.second, Rect{pos(0) / mCurrentMap.getTileWidth(), pos(1) / mCurrentMap.getTileHeight()}, mManager.GetTexture(mCurrentSelection.first));
      }

      update();
    }

    void GLContext::mouseReleaseEvent(QMouseEvent *e)
    {

    }

    GLContext::~GLContext()
    {
      delete mRenderer2D;
    }

  }
}
