#include "GL/glew.h"

#include "glcontext.h"

#include <QSurfaceFormat>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QApplication>
#include <QMimeData>

#include "Sprite.h"
#include "Texture.h"
#include "IRenderable.h"
#include "commands/CInsertTiles.h"
#include "commands/cchangeworkinglayer.h"

#include <vector>



namespace talga
{
  namespace editor
  {

    GLContext::GLContext(QWidget* parent)
      : QOpenGLWidget(parent)
      , camera{width(), height()}
      , mRenderer2D{nullptr}
      , mTileLayer{nullptr, width(), height()}
      , mSpriteLayer{nullptr, width(), height()}
      , mSelectionLayer{nullptr, width(), height()}
      , mManager{}
      , mCurrentMap()
      , mCurrentSelection{"NULL", std::vector<iPnt>{}}
      , mShift{false}
      , mIsMouseDown{false}
      , mPreviousMousePos{0.0f, 0.0f, 0.0f}
      , mCurrentMapLayerIndex{-1}
    {
      QSurfaceFormat format;
      format.setVersion(3, 3);
      format.setProfile(QSurfaceFormat::CoreProfile);
      setFormat(format);
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
      mSelectionLayer.setRenderer(mRenderer2D);
      mSelectionLayer.setProjectionMatrix(width(), height());

      camera.getBox().setW(width());
      camera.getBox().setH(height());
      camera.update(0);

      mRenderer2D->setCamera(&camera);

      //mManager.AddTexture("../../../assets/sprite_sheet.png");
      emit sig_loadAsset(QString("../../../assets/sprite_sheet.png"));

      mCurrentMap = *mManager.AddMap("../../../assets/test.map");

      mCurrentMap.insertSheet(mManager.GetTexture("sprite_sheet.png"));

      emit sig_updateLayerStack(&mCurrentMap);

      mTileLayer.add(&mCurrentMap);

      camera.getBox().setX(100);
      camera.getBox().setY(200);

      if (mCurrentMap.getLayers()->size() > 0)
      {
        mCurrentMapLayerIndex = 0;
      }
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

#ifdef TALGA_WINDOWS_BUILD
      QString fileName = path.split("\\").at(path.split("\\").size() - 1);
#endif

      TALGA_MSG(std::string("file ending: ") + fileEnding.toStdString());

      if ( fileEnding == "png" )
      {
        mManager.AddTexture(path.toStdString());
      }

      else if (fileEnding == "tmap")
      {
        mManager.AddMap(path.toStdString());
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

      auto tiles = mCurrentMap.getTiles(mCurrentSelection.second, mManager.GetTexture(mCurrentSelection.first));

      mSelectionLayer.clear();
      mSelectionRender.clear();
      mSelectionRender.reserve(tiles.size());
      for (auto& t : tiles)
      {
        mSelectionRender.push_back(Sprite{t.first, mCurrentMap.getTileWidth(), mCurrentMap.getTileHeight(), 0.5f, t.second});
        mSelectionLayer.add(&mSelectionRender.back());
      }
    } 

    void GLContext::sl_updateGL()
    {
      update();
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

      mSelectionLayer.getRenderer()->tStackPush(camera.getCameraMat());
      mSelectionLayer.render();
      mSelectionLayer.getRenderer()->tStackPop();
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
      if (e->button() == Qt::LeftButton)
      {
        mIsMouseDown = true;
        mPreviousMousePos = vec3(e->x(), e->y(), 1.0f);
        mStartNewHistoryItem = true;

        if (!mShift && !(mCurrentSelection.first == "NULL"))
        {
          mStartPos = e->pos();
          vec3 pos = camera.screenToWorld(vec3{(F32)e->x(), (F32)e->y(), 1.0f});

          if (pos(0) >= 0 && pos(0) < mCurrentMap.getTileWidth() * mCurrentMap.getWidth()
              && pos(1) >= 0 && pos(1) < mCurrentMap.getTileHeight() * mCurrentMap.getHeight())
          {
            std::vector<iPnt> tiles;

            //mCurrentMap.insertTile(mCurrentSelection.second, Rect{pos(0) / mCurrentMap.getTileWidth(), pos(1) / mCurrentMap.getTileHeight()}, mManager.GetTexture(mCurrentSelection.first));
            emit sig_updateHistoryMacro(true);
            emit sig_addUndoCommand(new CInsertTiles(&mCurrentMap, mCurrentMap.getTiles(mCurrentSelection.second, mManager.GetTexture(mCurrentSelection.first)),
                 iPnt(pos(0) / mCurrentMap.getTileWidth(), pos(1) / mCurrentMap.getTileHeight()), mCurrentSelection.second));
          }
        }
        mStartNewHistoryItem = false;

      }
    }

    void GLContext::mouseMoveEvent(QMouseEvent *e)
    {
      if (mIsMouseDown && !mShift)
      {
        mStartPos = e->pos();
        vec3 pos = camera.screenToWorld(vec3{(F32)e->x(), (F32)e->y(), 1.0f});

        if (pos(0) >= 0 && pos(0) < mCurrentMap.getTileWidth() * mCurrentMap.getWidth()
            && pos(1) >= 0 && pos(1) < mCurrentMap.getTileHeight() * mCurrentMap.getHeight())
        {
          if ( !(mCurrentSelection.first == "NULL") )
          {
            emit sig_addUndoCommand(new CInsertTiles(&mCurrentMap, mCurrentMap.getTiles(mCurrentSelection.second, mManager.GetTexture(mCurrentSelection.first)),
                                                    iPnt(pos(0) / mCurrentMap.getTileWidth(), pos(1) / mCurrentMap.getTileHeight()), mCurrentSelection.second));

          }

          update();
        }
      }
      else if (mIsMouseDown && mShift)
      {
        vec3 currentMousePos = vec3(e->x(), e->y(), 1.0f);
        vec3 dmouse = camera.screenToWorld(currentMousePos) - camera.screenToWorld(mPreviousMousePos);
        camera.getBox().setPosition(camera.getBox().getPosition() - dmouse);
        camera.update(0);
        mPreviousMousePos = currentMousePos;

        update();
      }

      // show transparent selection under mouse
      if ( !(mCurrentSelection.first == "NULL"))
      {
        I32 tW = mCurrentMap.getTileWidth();
        I32 tH = mCurrentMap.getTileHeight();

        if (mSelectionRender.size() > 0)
        {
          I32 greatestX = 0;
          I32 greatestY = 0;
          I32 smallestX = 100000;
          I32 smallestY = 100000;
          I32 greatestXIndex = 0;
          I32 greatestYIndex = 0;
          I32 smallestXIndex = 0;
          I32 smallestYIndex = 0;
          I32 count = 0;

          for (const auto& pnt : mCurrentSelection.second)
          {
            if (pnt.x() > greatestX)
            {
              greatestX = pnt.x();
              greatestXIndex = count;
            }

            if (pnt.y() > greatestY)
            {
              greatestY = pnt.y();
              greatestYIndex = count;
            }

            if (pnt.x() < smallestX)
            {
              smallestX = pnt.x();
              smallestXIndex = pnt.x();
            }

            if (pnt.y() < smallestY)
            {
              smallestY = pnt.y();
              smallestYIndex = pnt.y();
            }

            ++count;
          }

          for (const auto& pnt : mCurrentSelection.second)
          {
            vec3 setTo(e->x(), e->y(), 1.0f);
            setTo = camera.screenToWorld(setTo);
            setTo[0] -= (I32)setTo(0) % tW - (0.5f * tW);
            setTo[1] -= (I32)setTo(1) % tH - (0.5f * tH);
            setTo[0] += (pnt.x() - smallestX) * tW;
            setTo[1] += (pnt.y() - smallestY) * tH;

            setTo[0] = (I32)setTo(0);
            setTo[1] = (I32)setTo(1);

            mSelectionRender[ (pnt.y() - smallestY) * (greatestX - smallestX + 1) + (pnt.x() - smallestX)].getBox().setPosition(setTo);
            mSelectionRender[ (pnt.y() - smallestY) * (greatestX - smallestX + 1) + (pnt.x() - smallestX)].getBox().updateVertsPosition();
          }

          update();
        }
      }

    }

    void GLContext::mouseReleaseEvent(QMouseEvent *e)
    {
      if (e->button() == Qt::LeftButton)
      {
        mIsMouseDown = false;
        emit sig_updateHistoryMacro(false);
      }
    }

    void GLContext::wheelEvent(QWheelEvent *e)
    {
      F32 amount = e->delta() / 400.0f;

      camera.getBox().setScaleX(camera.getBox().getScaleX() - amount);
      camera.getBox().setScaleY(camera.getBox().getScaleY() - amount);

      if (camera.getBox().getScaleX() <= 0.1f || camera.getBox().getScaleY() <= 0.1f)
      {
        camera.getBox().setScaleX(0.1f);
        camera.getBox().setScaleY(0.1f);
      }
      camera.update(0);


      update();
    }

    void GLContext::keyPressEvent(QKeyEvent *e)
    {
      if (e->key() == Qt::Key_Shift)
      {
        mShift = true;
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
      }

      if ((e->key() == Qt::Key_Z)  && (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)))
      {
        emit sig_ctrlz();
      }

      if ((e->key() == Qt::Key_Y)  && (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)))
      {
        emit sig_ctrly();
      }
    }

    void GLContext::keyReleaseEvent(QKeyEvent *e)
    {
      if (e->key() == Qt::Key_Shift)
      {
        mShift = false;
        QApplication::restoreOverrideCursor();
      }

    }

    GLContext::~GLContext()
    {
      mSelectionLayer.clear();
      mSelectionRender.clear();

      delete mRenderer2D;
    }

  }
}
