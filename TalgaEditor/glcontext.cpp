#include "GL/glew.h"

#include "glcontext.h"
#include <QSurfaceFormat>
#include "Sprite.h"


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
    , manager{}
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    setFormat(format);

   // paintGL();
}
Sprite* temp = nullptr;
void GLContext::initializeGL()
{
    glewExperimental = true;

    GLenum err = glewInit();
    TALGA_ASSERT(err == GLEW_OK, "Failed to initialize glew, exiting...");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mRenderer2D = new Renderer("../../../assets/shaders/renderer2d.vert", "../../../assets/shaders/renderer2d.frag");

    manager.AddTexture("../../../assets/sprite_sheet.png");

    temp = new Sprite{manager.GetTexture("sprite_sheet.png")};
    mSpriteLayer.add(temp);

    mSpriteLayer.setRenderer(mRenderer2D);

    camera.getBox().setW(width());
    camera.getBox().setH(height());
    camera.update(0);

    mRenderer2D->setCamera(&camera);
    mSpriteLayer.setProjectionMatrix(width(), height());
}

void GLContext::paintGL()
{

    mSpriteLayer.render();
}

GLContext::~GLContext()
{
    delete temp;
    delete mRenderer2D;
}

}
}
