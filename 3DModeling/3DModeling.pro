# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = 3DModeling
DESTDIR = ../Win32/Debug
QT += core opengl widgets gui
CONFIG += debug console
DEFINES += QT_DLL QT_WIDGETS_LIB QT_OPENGL_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
LIBS += -lglew32s \
    -lOpenGL32 \
    -l$(NOINHERIT)
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(3DModeling.pri)
win32:RC_FILE = 3DModeling.rc
