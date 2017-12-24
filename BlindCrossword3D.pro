QT += core gui widgets printsupport
QT -= webkitwidgets

TARGET = BlindCrossword3D
TEMPLATE = app

# Disables text-to-speech functionality
#DEFINES += NO_TTS

# Custom paths
win32 {
    DEFINES += _USING_V110_SDK71_ # Fixes some errors in atl headers
    QMAKE_CXXFLAGS -= -Zc:strictStrings # Fixes some errors in sapi.h
    INCLUDEPATH += "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include" # SAPI
    INCLUDEPATH += "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\atlmfc\include" # ATL
}

# Custom OSX configuration
macx {
    INCLUDEPATH += /System/Library/Frameworks/AppKit.framework/Versions/C/Headers
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

# Include the headers, sources, UI forms
include($$PWD/sources/sources.pri)

# Include the resources, crossword files, icons etc
include($$PWD/resources/resources.pri)
