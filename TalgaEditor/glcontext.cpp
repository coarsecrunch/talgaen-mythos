#include "glcontext.h"
#include <QSurfaceFormat>

namespace talga
{
namespace editor
{

GLContext::GLContext(QWidget* parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    setFormat(format);
}

GLContext::~GLContext()
{

}

}
}
