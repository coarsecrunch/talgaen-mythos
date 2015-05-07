#ifndef GLCONTEXT_H
#define GLCONTEXT_H

#include <QOpenGLWidget>

#include "Renderer.h"
#include "Layer.h"
#include "Camera.h"

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

    void initializeGL();
    void paintGL();
protected:
    Camera camera;

    Layer mTileLayer;
    Layer mSpriteLayer;
    Renderer* mRenderer2D;
    AssetManager manager;


};

}
}
#endif // GLCONTEXT_H
