TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/freetype2/
LIBS += -lGL -lGLEW -lglfw -lfreeimage -lfreetype
SOURCES += main.cpp

HEADERS += \
    background.h \
    block.h \
    structs.h \
    shaders.h \
    text.h \
    character.h
