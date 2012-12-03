#-------------------------------------------------
#
# Project created by QtCreator 2012-07-31T21:41:03
#
#-------------------------------------------------

QT       += core gui widgets printsupport
QT       -= webkitwidgets

TARGET = BlindCrossword3D
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    word.cpp \
    grid3dgraphicsscene.cpp \
    uivec3.cpp \
    crosswordentry3d.cpp \
    direction.cpp \
    letter.cpp \
    puzzleloader.cpp \
    grid3dgraphicsview.cpp \
    wordtablemodel.cpp \
    guessworddialog.cpp \
    wordtableviewcontroller.cpp \
    lettergrid.cpp \
    graphicsgriditem.cpp \
    itexttospeech.cpp \
    ttsimpl.cpp \
    quitdialog.cpp \
    shortcutkeys.cpp \
    filedialog.cpp \
    xwc3dloader.cpp \
    xwcloader.cpp \
    xwcr3dloader.cpp \
    puzzlebase.cpp \
    puzzleloaderinterface.cpp \
    emailer.cpp

HEADERS  += mainwindow.h \
    word.h \
    grid3dgraphicsscene.h \
    uivec3.h \
    crosswordentry3d.h \
    letter.h \
    puzzleloader.h \
    grid3dgraphicsview.h \
    wordtablemodel.h \
    guessworddialog.h \
    wordtableviewcontroller.h \
    lettergrid.h \
    graphicsgriditem.h \
    itexttospeech.h \
    singleton.h \
    ttsimpl.h \
    quitdialog.h \
    shortcutkeys.h \
    filedialog.h \
    xwc3dloader.h \
    xwcloader.h \
    xwcr3dloader.h \
    puzzlebase.h \
    puzzleloaderinterface.h \
    fileformats.h \
    crosswordtypes.h \
    direction.h \
    emailer.h

Win32 {
QMAKE_LFLAGS_RELEASE += -static-libgcc
}

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
