QT       += core gui widgets

INCLUDEPATH += $$PWD/src

TARGET = gcdebugger
TEMPLATE = app

SOURCES += $$PWD/main.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/gcwidget.cpp

HEADERS += $$PWD/mainwindow.h \
           $$PWD/gcwidget.h