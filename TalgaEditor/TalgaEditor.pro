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
    historyviewer.cpp \
    layerstack.cpp \
    globals.cpp

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
    historyviewer.h \
    layerstack.h \
    globals.h


FORMS    += mainwindow.ui \
    wrongextdialog.ui

DISTFILES += \
    ../assets/sandbox/Archimedes/Archimedes.autosave.scml \
    ../assets/sandbox/Archimedes/Archimedes.scml \
    ../assets/sandbox/Archimedes/NewProject.autosave.scml \
    ../assets/sandbox/Archimedes/Archimedes.fla \
    ../assets/sandbox/talga/talga.fla \
    ../assets/sandbox/Archimedes/body_sym.png \
    ../assets/sandbox/Archimedes/calf.png \
    ../assets/sandbox/Archimedes/foot.png \
    ../assets/sandbox/Archimedes/forearm.png \
    ../assets/sandbox/Archimedes/hand.png \
    ../assets/sandbox/Archimedes/head_sym.png \
    ../assets/sandbox/Archimedes/neck_sym.png \
    ../assets/sandbox/Archimedes/shoulder.png \
    ../assets/sandbox/Archimedes/thig.png \
    ../assets/sandbox/blah.png \
    ../assets/sandbox/scrap4.png \
    ../assets/charactersheet.png \
    ../assets/notex.png \
    ../assets/sprite_sheet.png \
    ../assets/sprite_sheet2.png \
    ../assets/talgasheet.png \
    ../assets/sandbox/whyisthisnotworking.xcf \
    ../assets/test.map \
    ../assets/shaders/renderer2d.frag \
    ../assets/shaders/renderer2d.vert \
    ../assets/scripts/script.lua

RESOURCES += \
    ../assets/icons/icons.qrc


unix:!macx: LIBS += -lGLEW

unix:!macx: LIBS += -lglfw

unix:!macx: LIBS += -llua5.2

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -llua5.2

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/x86_64-linux-gnu/liblua5.2.a

win32: LIBS += -L$$PWD/../lib/win64/ -lglew32

INCLUDEPATH += $$PWD/../lib/win64
DEPENDPATH += $$PWD/../lib/win64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/glew32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libglew32.a

win32: LIBS += -L$$PWD/../lib/win64/ -lglfw3dll

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/glfw3dll.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libglfw3dll.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/win64/ -llua52
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/win64/ -llua52d


win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liblua52.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liblua52d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/lua52.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/lua52d.lib

win32:CONFIG(release, debug|release): LIBS += "-L$$PWD/C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64/" -lOpenGL32
else:win32:CONFIG(debug, debug|release): LIBS += "-L$$PWD/C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64/" -lOpenGL32d



