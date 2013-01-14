#-------------------------------------------------
#
# Project created by QtCreator 2012-07-31T21:41:03
#
#-------------------------------------------------

QT       += core gui widgets printsupport
QT       -= webkitwidgets

TARGET = BlindCrossword3D
TEMPLATE = app

SOURCES += src/xwcloader.cpp \
    src/xwc3dloader.cpp \
    src/version.cpp \
    src/utilities.cpp \
    src/uivec3.cpp \
    src/shortcutkeys.cpp \
    src/quitdialog.cpp \
    src/printer.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/letter.cpp \
    src/itexttospeech.cpp \
    src/idlereminder.cpp \
    src/guessworddialog.cpp \
    src/graphicalgridview.cpp \
    src/graphicalgridscene.cpp \
    src/graphicalgriditem.cpp \
    src/filedialog.cpp \
    src/emailer.cpp \
    src/crosswordword.cpp \
    src/crosswordloader.cpp \
    src/crosswordgrid.cpp \
    src/crosswordentrytableviewcontroller.cpp \
    src/crosswordentrytablemodel.cpp \
    src/crosswordentry.cpp \
    src/crosswordbase.cpp \
    src/cluereader.cpp \

HEADERS  += src/xwcloader.h \
    src/xwc3dloader.h \
    src/version.h \
    src/utilities.h \
    src/uivec3.h \
    src/singleton.h \
    src/shortcutkeys.h \
    src/quitdialog.h \
    src/printer.h \
    src/mainwindow.h \
    src/letter.h \
    src/itexttospeech.h \
    src/idlereminder.h \
    src/guessworddialog.h \
    src/graphicalgridview.h \
    src/graphicalgridscene.h \
    src/graphicalgriditem.h \
    src/filedialog.h \
    src/emailer.h \
    src/crosswordword.h \
    src/crosswordstatus.h \
    src/crosswordloader.h \
    src/crosswordgrid.h \
    src/crosswordfileformats.h \
    src/crosswordentrytableviewcontroller.h \
    src/crosswordentrytablemodel.h \
    src/crosswordentry.h \
    src/crosswordbase.h \
    src/cluereader.h \

win32 {
    SOURCES += src/ttsimplwindows.cpp
    HEADERS += src/ttsimplwindows.h
}

macx {
    SOURCES += src/ttsimplmac.cpp
    HEADERS += src/ttsimplmac.h
}

linux {
    SOURCES += src/ttsimpllinux.cpp
    HEADERS += src/ttsimpllinux.h
}

DEPENDPATH += . src
INCLUDEPATH += . src

CONFIG += x86 x86_64

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
