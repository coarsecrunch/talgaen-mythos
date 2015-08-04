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
    ../include/freetype/

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
    ../Engine/ext/freetype-gl/platform.c \
    ../Engine/ext/freetype-gl/texture-atlas.c \
    ../Engine/ext/freetype-gl/texture-font.c \
    ../Engine/ext/freetype-gl/vector.c \
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
    ../Engine/ext/freetype-gl/freetype-gl.h \
    ../Engine/ext/freetype-gl/opengl.h \
    ../Engine/ext/freetype-gl/platform.h \
    ../Engine/ext/freetype-gl/texture-atlas.h \
    ../Engine/ext/freetype-gl/texture-font.h \
    ../Engine/ext/freetype-gl/vec234.h \
    ../Engine/ext/freetype-gl/vector.h \
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
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/win64/ -llua52


win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liblua52.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/liblua52.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/lua52.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../lib/win64/lua52.lib

win32:CONFIG(release, debug|release): LIBS += "-L$$PWD/C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64/" -lOpenGL32
else:win32:CONFIG(debug, debug|release): LIBS += "-L$$PWD/C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64/" -lOpenGL32




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

win32: LIBS += -L$$PWD/../lib/win64/ -lfreetype26MT

INCLUDEPATH += $$PWD/../lib/win64
DEPENDPATH += $$PWD/../lib/win64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/freetype26MT.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../lib/win64/libfreetype26MT.a

DISTFILES += \
    ../Engine/ext/freetype-gl/fonts/amiri-regular.ttf \
    ../Engine/ext/freetype-gl/fonts/fireflysung.ttf \
    ../Engine/ext/freetype-gl/fonts/Liberastika-Regular.ttf \
    ../Engine/ext/freetype-gl/fonts/Lobster-Regular.ttf \
    ../Engine/ext/freetype-gl/fonts/LuckiestGuy.ttf \
    ../Engine/ext/freetype-gl/fonts/OldStandard-Regular.ttf \
    ../Engine/ext/freetype-gl/fonts/SourceCodePro-Regular.ttf \
    ../Engine/ext/freetype-gl/fonts/SourceSansPro-Regular.ttf \
    ../Engine/ext/freetype-gl/fonts/Vera.ttf \
    ../Engine/ext/freetype-gl/fonts/VeraMoBd.ttf \
    ../Engine/ext/freetype-gl/fonts/VeraMoBI.ttf \
    ../Engine/ext/freetype-gl/fonts/VeraMoIt.ttf \
    ../Engine/ext/freetype-gl/fonts/VeraMono.ttf \
    ../Engine/ext/freetype-gl/fonts/amiri-regular_LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/fireflysung-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/Liberastika-Regular-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/Lobster-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/LuckiestGuy-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/OldStandard-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/SourceCodePro-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/SourceSansPro-LICENSE.txt \
    ../Engine/ext/freetype-gl/fonts/VERA-COPYRIGHT \
    ../assets/scripts/globals.lua \
    ../assets/scripts/sandboxx.lua \
    ../assets/scripts/script.lua \
    ../assets/scripts/talga.lua \
    ../assets/scripts/testmod.lua
