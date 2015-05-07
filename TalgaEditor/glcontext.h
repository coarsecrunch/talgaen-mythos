#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <QOpenGLWidget>

namespace talga
{
namespace editor
{

class GLContext : public QOpenGLWidget
{
    Q_OBJECT
public:
    GLContext(QWidget* parent);
    ~GLContext();
};

}
}
#endif // GLCONTEXT_H
