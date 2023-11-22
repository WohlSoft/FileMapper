TEMPLATE = app
CONFIG -= qt
CONFIG += console

TARGET = FileMapper-Test
DESTDIR = $$PWD/bin

include(FileMapper.pri)

SOURCES += \
    test/test.cpp
