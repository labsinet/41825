QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = GasMeterAccounting
TEMPLATE = app

SOURCES += \
    main.cpp \
    adddialog.cpp

HEADERS += \
    adddialog.h

DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj
UI_DIR = obj