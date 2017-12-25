QT += core gui widgets printsupport texttospeech
QT -= webkitwidgets

TARGET = BlindCrossword3D
TEMPLATE = app

# Disables text-to-speech functionality
#DEFINES += NO_TTS

# Include the headers, sources, UI forms
include($$PWD/sources/sources.pri)

# Include the resources, crossword files, icons etc
include($$PWD/resources/resources.pri)
