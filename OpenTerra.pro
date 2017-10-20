TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGL -lGLEW -lglfw -lfreeimage
SOURCES += main.cpp

HEADERS += \
    background.h \
    block.h \
    structs.h \
    shaders.h \
    text.h
