# Platform-dependent application icons
win32 {
    RC_FILE = $$PWD/winicon.rc
}
macx {
    ICON = $$PWD/icon.icns
}
linux {
    # TODO gnome, kde icons
}
RESOURCES += $$PWD/resources.qrc