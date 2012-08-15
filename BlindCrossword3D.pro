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
    direction.cpp \
    puzzle3d.cpp \
    letter.cpp \
    puzzleloader.cpp \
    grid3dgraphicsview.cpp \
    wordtablemodel.cpp \
    clue.cpp \
    guessworddialog.cpp \
    wordtableviewcontroller.cpp \
    lettergrid.cpp \
    graphicsgriditem.cpp \
    itexttospeech.cpp \
    singleton.cpp \
    ttsimpl.cpp \
    quitdialog.cpp \
    shortcutkeys.cpp \
    filedialog.cpp

HEADERS  += mainwindow.h \
    word.h \
    grid3dgraphicsscene.h \
    uivec3.h \
    crosswordentry3d.h \
    direction.h \
    puzzle3d.h \
    letter.h \
    puzzleloader.h \
    grid3dgraphicsview.h \
    wordtablemodel.h \
    clue.h \
    guessworddialog.h \
    wordtableviewcontroller.h \
    lettergrid.h \
    graphicsgriditem.h \
    itexttospeech.h \
    singleton.h \
    ttsimpl.h \
    quitdialog.h \
    shortcutkeys.h \
    filedialog.h



FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
