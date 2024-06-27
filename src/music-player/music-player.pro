QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
VERSION += 1.0.0.0
RC_ICONS += logo.ico

include($$PWD/basewidget/basewidget.pri)
include($$PWD/configuration/configuration.pri)
include($$PWD/database/database.pri)
include($$PWD/player/player.pri)

SOURCES += \
    favoritepage.cpp \
    homepage.cpp \
    librarypage.cpp \
    main.cpp \
    mainwindow.cpp \
    settingspage.cpp

HEADERS += \
    basepage.h \
    favoritepage.h \
    homepage.h \
    librarypage.h \
    mainwindow.h \
    settingspage.h

FORMS += \
    favoritepage.ui \
    homepage.ui \
    librarypage.ui \
    mainwindow.ui \
    settingspage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    music-player.qrc
