#-------------------------------------------------
#
# Project created by QtCreator 2012-07-31T21:41:03
#
#-------------------------------------------------

QT       += core gui

TARGET = BlindCrossword3D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    word.cpp \
    grid3dgraphicsscene.cpp \
    uivec3.cpp \
    crosswordentry3d.cpp \
    hint.cpp \
    direction.cpp \
    puzzle3d.cpp \
    letter.cpp \
    puzzleloader.cpp \
    grid3dgraphicsview.cpp \
    wordtablemodel.cpp \
    wordtableview.cpp \
    eventfilters.cpp

HEADERS  += mainwindow.h \
    word.h \
    grid3dgraphicsscene.h \
    uivec3.h \
    crosswordentry3d.h \
    hint.h \
    direction.h \
    puzzle3d.h \
    letter.h \
    puzzleloader.h \
    grid3dgraphicsview.h \
    wordtablemodel.h \
    wordtableview.h \
    eventfilters.h

FORMS    += mainwindow.ui
