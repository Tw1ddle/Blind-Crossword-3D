#-------------------------------------------------
#
# Project created by QtCreator 2012-07-31T21:41:03
#
#-------------------------------------------------

QT       += core gui widgets printsupport
QT       -= webkitwidgets

TARGET = BlindCrossword3D
TEMPLATE = app

#DEFINES += NO_TTS

# Custom paths
win32 {
    INCLUDEPATH += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include" # SAPI
    INCLUDEPATH += "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\atlmfc\include" # ATL
}

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
    src/ttsbase.cpp \
    src/speechlogwidget.cpp \
    src/ttsstub.cpp


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
    src/ttsbase.h \
    src/speechlogwidget.h \
    src/ttsstub.h

# Platform-dependent text to speech
win32 {
    SOURCES += src/ttsimplwindows.cpp
    HEADERS += src/ttsimplwindows.h
}

macx {
    OBJECTIVE_SOURCES += src/ttsimplmac.mm
    HEADERS += src/ttsimplmac.h

    INCLUDEPATH += /System/Library/Frameworks/AppKit.framework/Versions/C/Headers
    OBJECTIVE_HEADERS +=

    MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk

    if(!exists($$MAC_SDK)) {
        error("The selected Mac OSX SDK does not exist at $$MAC_SDK!")
    }

    APPKIT_PATH = $$MAC_SDK/System/Library/Frameworks/AppKit.framework/Versions/C/Headers

    if(!exists($$APPKIT_PATH)) {
        error("The path to the AppKit framework does not exist, $$APPKIT_PATH!")
    }

    INCLUDEPATH += APPKIT_PATH
    DEPENDPATH += APPKIT_PATH

    LIBS += -framework AppKit -framework Foundation
}

linux {
    SOURCES += src/ttsimpllinux.cpp
    HEADERS += src/ttsimpllinux.h
}

# Platform-dependent application icons
win32 {
    RC_FILE = winicon.rc
}

macx {
    ICON = icon.icns
}

linux {
    # TODO gnome, kde icons
}

INCLUDEPATH += . src
DEPENDPATH += . src

CONFIG += x86 x86_64

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
