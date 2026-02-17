QT       += core gui widgets

INCLUDEPATH += $$PWD/src

TARGET = cmexplorer
TEMPLATE = app

SOURCES += $$PWD/main.cpp \
           $$PWD/mainwindow.cpp \
           $$PWD/volfilelistwidget.cpp \
           $$PWD/volfilelistwidgetitem.cpp \
           $$PWD/volfile.cpp \
           $$PWD/volentry.cpp \
           $$PWD/spritevolentry.cpp \
           $$PWD/gcvolentry.cpp

HEADERS += $$PWD/mainwindow.h \
           $$PWD/volfilelistwidget.h \
           $$PWD/volfilelistwidgetitem.h \
           $$PWD/imagelabel.h \
           $$PWD/volfile.h \
           $$PWD/volentry.h \
           $$PWD/spritevolentry.h \
           $$PWD/gcvolentry.h