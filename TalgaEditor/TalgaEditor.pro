#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T20:47:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console \
    c++11


TARGET = TalgaEditor
TEMPLATE = app
INCLUDEPATH += ../Engine/ \
    ../Engine/Math/ \
    ../include/

DEFINES += GLFW_DLL \
    TALGA_ASSERTIONS_ENABLED \
    TALGA_UBUNTU_BUILD \
    TALGA_WARNINGS_ENABLED \
    TALGA_MESSAGES_ENABLED



SOURCES += ../TalgaEditor/main.cpp \
        mainwindow.cpp \
    assetlist.cpp \
    assetviewer.cpp \
    wrongextdialog.cpp \
    glcontext.cpp \
    ../Engine/Math/Matrix3x3.cpp \
    ../Engine/Math/Matrix4x4.cpp \
    ../Engine/Math/Operations.cpp \
    ../Engine/Math/RandomGen.cpp \
    ../Engine/Math/Vector2.cpp \
    ../Engine/Math/Vector3.cpp \
    ../Engine/Math/Vector4.cpp \
    ../Engine/AnimSet.cpp \
    ../Engine/AssetManager.cpp \
    ../Engine/Camera.cpp \
    ../Engine/Clock.cpp \
    ../Engine/Collider.cpp \
    ../Engine/Collision.cpp \
    ../Engine/ESubject.cpp \
    ../Engine/Game.cpp \
    ../Engine/Interpolation.cpp \
    ../Engine/Layer.cpp \
    ../Engine/LuaEngine.cpp \
    ../Engine/Map.cpp \
    ../Engine/Observer.cpp \
    ../Engine/Particle.cpp \
    ../Engine/ParticleEmitter.cpp \
    ../Engine/Rectangle.cpp \
    ../Engine/Renderer.cpp \
    ../Engine/Rendering.cpp \
    ../Engine/Sprite.cpp \
    ../Engine/Texture.cpp \
    ../Engine/Transformation2D.cpp \
    ../Engine/Window.cpp \
    editormap.cpp \
    commands/CInsertTiles.cpp \
    historyviewer.cpp

HEADERS  += mainwindow.h \
    assetlist.h \
    assetviewer.h \
    wrongextdialog.h \
    glcontext.h \
    ../Engine/Math/Interpolation.h \
    ../Engine/Math/MathCommon.h \
    ../Engine/Math/Matrix3x3.h \
    ../Engine/Math/Matrix4x4.h \
    ../Engine/Math/Operations.h \
    ../Engine/Math/RandomGen.h \
    ../Engine/Math/Vector2.h \
    ../Engine/Math/Vector3.h \
    ../Engine/Math/Vector4.h \
    ../Engine/AnimSet.h \
    ../Engine/AssetManager.h \
    ../Engine/Camera.h \
    ../Engine/Clock.h \
    ../Engine/Cmn.h \
    ../Engine/Collider.h \
    ../Engine/Collision.h \
    ../Engine/ESubject.h \
    ../Engine/Event.h \
    ../Engine/Game.h \
    ../Engine/IDynamic.h \
    ../Engine/IRenderable.h \
    ../Engine/Layer.h \
    ../Engine/LuaEngine.h \
    ../Engine/Map.h \
    ../Engine/Observer.h \
    ../Engine/Particle.h \
    ../Engine/ParticleEmitter.h \
    ../Engine/Point.h \
    ../Engine/Rect.h \
    ../Engine/Rectangle.h \
    ../Engine/Renderer.h \
    ../Engine/Rendering.h \
    ../Engine/Sprite.h \
    ../Engine/tassert.h \
    ../Engine/Texture.h \
    ../Engine/Transformation2D.h \
    ../Engine/Window.h \
    editormap.h \
    commands/CInsertTiles.h \
    historyviewer.h


FORMS    += mainwindow.ui \
    wrongextdialog.ui

win32: LIBS += -lOpenGL32

win32: LIBS += -L$$PWD/../lib/glew/ -lglew32

INCLUDEPATH += $$PWD/../lib/glew
DEPENDPATH += $$PWD/../lib/glew

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/glew/glew32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/glew/libglew32.a

win32: LIBS += -L$$PWD/../lib/glfw/ -lglfw3

INCLUDEPATH += $$PWD/../lib/glfw
DEPENDPATH += $$PWD/../lib/glfw

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/glfw/glfw3.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/glfw/libglfw3.a

win32: LIBS += -L$$PWD/../lib/lua/ -llua53

INCLUDEPATH += $$PWD/../lib/lua
DEPENDPATH += $$PWD/../lib/lua

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/lua/lua53.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/lua/liblua53.a

win32: LIBS += -L$$PWD/../lib/glfw/ -lglfw3dll

INCLUDEPATH += $$PWD/../lib/glfw
DEPENDPATH += $$PWD/../lib/glfw

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/glfw/glfw3dll.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/glfw/libglfw3dll.a

DISTFILES +=

RESOURCES += \
    ../assets/icons/icons.qrc


unix:!macx: LIBS += -lGLEW

unix:!macx: LIBS += -lglfw

unix:!macx: LIBS += -llua5.2

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -llua5.2

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/liblua5.2.a
