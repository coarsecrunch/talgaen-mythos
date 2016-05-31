#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T20:47:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
CONFIG += c++11

TARGET = TalgaEditor
TEMPLATE = app
INCLUDEPATH += ../Engine/ \
    ../Engine/Math/ \
    ../include/ \

DEFINES -= UNICODE

DEFINES += GLFW_DLL \
    TALGA_ASSERTIONS_ENABLED \
    TALGA_WARNINGS_ENABLED \
    TALGA_MESSAGES_ENABLED \
    TALGA_QT_BUILD \

win32: DEFINES += TALGA_WINDOWS_BUILD
win32: DEFINES += _CRT_SECURE_NO_WARNINGS
unix:!macx: DEFINES += TALGA_UBUNTU_BUILD



SOURCES += ../TalgaEditor/main.cpp \
        mainwindow.cpp \
    assetlist.cpp \
    assetviewer.cpp \
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
    ../Engine/Collision.cpp \
    ../Engine/Game.cpp \
    ../Engine/Interpolation.cpp \
    ../Engine/Layer.cpp \
    ../Engine/LuaEngine.cpp \
    ../Engine/Map.cpp \
    ../Engine/Particle.cpp \
    ../Engine/ParticleEmitter.cpp \
    ../Engine/Rectangle.cpp \
    ../Engine/Renderer.cpp \
    ../Engine/Rendering.cpp \
    ../Engine/Sprite.cpp \
    ../Engine/Texture.cpp \
    ../Engine/Transformation2D.cpp \
    ../Engine/Window.cpp \
    ../Engine/label.cpp \
    ../Engine/renderableshapes.cpp \
    ../Engine/font.cpp \
    ../Engine/funkdefs.cpp \
    ../Engine/luadebugprompt.cpp \
    ../Engine/PhysicsComponent.cpp \
    ../Engine/sys.cpp \
    editormap.cpp \
    commands/CInsertTiles.cpp \
    historyviewer.cpp \
    layerstack.cpp \
    commands/cchangeworkinglayer.cpp \
    commands/caddlayer.cpp \
    commands/cremovelayer.cpp \
    gdata.cpp \
    commands/cchangelayerorder.cpp \
    newmapdialog.cpp \
    ../Engine/GameObject.cpp \
    ../Engine/Triangle.cpp \
    ../Engine/luareg.cpp \
    ../Engine/StateMachine.cpp \
    states/STileEdit.cpp \
    states/editstate.cpp \
    states/sgeomedit.cpp \
    ../Engine/Script.cpp \
    propertiesviewer.cpp



HEADERS  += \
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
    ../Engine/Collision.h \
    ../Engine/Game.h \
    ../Engine/IDynamic.h \
    ../Engine/IRenderable.h \
    ../Engine/Layer.h \
    ../Engine/LuaEngine.h \
    ../Engine/Map.h \
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
    ../Engine/sys.h \
    ../Engine/AAsset.h \
    ../Engine/label.h \
    ../Engine/renderableshapes.h \
    ../Engine/font.h \
    ../Engine/funkdefs.h \
    ../Engine/luadebugprompt.h \
    ../Engine/PhysicsComponent.h \
    mainwindow.h \
    assetlist.h \
    assetviewer.h \
    glcontext.h \
    editormap.h \
    commands/CInsertTiles.h \
    historyviewer.h \
    layerstack.h \
    commands/cchangeworkinglayer.h \
    commands/caddlayer.h \
    commands/cremovelayer.h \
    gdata.h \
    commands/cchangelayerorder.h \
    newmapdialog.h \
    globals.h \
    ../Engine/GameObject.h \
    ../Engine/Triangle.h \
    ../Engine/luareg.h \
    ../Engine/StateMachine.h \
    states/STileEdit.h \
    states/editstate.h \
    states/sgeomedit.h \
    ../Engine/Script.h \
    propertiesviewer.h


FORMS    += mainwindow.ui \
    newmapdialog.ui

RESOURCES += \
    ../assets/icons/icons.qrc
win32:CONFIG(release, debug|release): LIBS += -'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/um/x64/' -lOpenGL32
else:win32:CONFIG(debug, debug|release): LIBS += -'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/um/x64/' -lOpenGL32

unix:!macx: LIBS += -lGLEW

unix:!macx: LIBS += -lglfw



INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu

win32: LIBS += -L$$PWD/../lib/win64/ -lglew32
win32: LIBS += -L$$PWD/../lib/win64/ -lglfw3
win32: LIBS += -L$$PWD/../lib/win64/ -lglfw3dll


INCLUDEPATH += $$PWD/../lib/win64
DEPENDPATH += $$PWD/../lib/win64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/glew32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libglew32.a

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/glfw3.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libglfw3.a

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/glfw3dll.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libglfw3dll.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/win64/ -llua53
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/win64/ -llua53


win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liblua53.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liblua53.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/lua53.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/lua53.lib




win32: LIBS += -L$$PWD/../lib/win64/ -lchipmunk

INCLUDEPATH += $$PWD/../lib/win64
DEPENDPATH += $$PWD/../lib/win64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/chipmunk.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libchipmunk.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/win64/ -loolua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/win64/ -looluad

INCLUDEPATH += $$PWD/../lib/win64
DEPENDPATH += $$PWD/../lib/win64

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liboolua.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/libooluad.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/oolua.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/ooluad.lib

INCLUDEPATH += $$PWD/../lib/win64
DEPENDPATH += $$PWD/../lib/win64

