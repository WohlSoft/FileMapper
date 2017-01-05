TEMPLATE = app
CONFIG -= qt
CONFIG += console

TARGET = FileMapper-Test
DESTDIR = $$PWD/bin

include(src/FileMapper.pri)

SOURCES += \
    test/test.cpp

