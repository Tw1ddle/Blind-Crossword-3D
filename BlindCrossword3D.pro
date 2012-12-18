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
    uivec3.cpp \
    letter.cpp \
    guessworddialog.cpp \
    itexttospeech.cpp \
    ttsimpl.cpp \
    quitdialog.cpp \
    shortcutkeys.cpp \
    filedialog.cpp \
    xwc3dloader.cpp \
    xwcloader.cpp \
    emailer.cpp \
    printer.cpp \
    idlereminder.cpp \
    utilities.cpp \
    version.cpp \
    graphicalgridview.cpp \
    graphicalgriditem.cpp \
    graphicalgridscene.cpp \
    crosswordbase.cpp \
    crosswordentry.cpp \
    crosswordgrid.cpp \
    crosswordloader.cpp \
    crosswordentrytablemodel.cpp \
    crosswordentrytableviewcontroller.cpp \
    cluereader.cpp \
    crosswordword.cpp

HEADERS  += mainwindow.h \
    uivec3.h \
    letter.h \
    guessworddialog.h \
    itexttospeech.h \
    singleton.h \
    ttsimpl.h \
    quitdialog.h \
    shortcutkeys.h \
    filedialog.h \
    xwc3dloader.h \
    xwcloader.h \
    fileformats.h \
    crosswordtypes.h \
    emailer.h \
    printer.h \
    idlereminder.h \
    utilities.h \
    version.h \
    cluereader.h \
    graphicalgridscene.h \
    graphicalgriditem.h \
    graphicalgridview.h \
    crosswordbase.h \
    crosswordentry.h \
    crosswordgrid.h \
    crosswordloader.h \
    crosswordentrytablemodel.h \
    crosswordentrytableviewcontroller.h \
    crosswordword.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
