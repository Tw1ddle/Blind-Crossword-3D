# Platform-dependent application icons
win32 {
    RC_FILE = $$PWD/winicon.rc
}
macx {
    ICON = $$PWD/icon.icns
}
linux {
}
RESOURCES += $$PWD/resources.qrc
