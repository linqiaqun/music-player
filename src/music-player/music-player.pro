QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
VERSION += 1.0.0.0
RC_ICONS += logo.ico

include($$PWD/basedialog/basedialog.pri)
include($$PWD/basewidget/basewidget.pri)
include($$PWD/clickablelabel/clickablelabel.pri)
include($$PWD/clickableprogressbar/clickableprogressbar.pri)
include($$PWD/configuration/configuration.pri)
include($$PWD/database/database.pri)
include($$PWD/flowlayout/flowlayout.pri)
include($$PWD/labeledit/labeledit.pri)
include($$PWD/logger/logger.pri)
include($$PWD/player/player.pri)
include($$PWD/taglibhelper/taglibhelper.pri)
include($$PWD/util/util.pri)

SOURCES += \
    VolumeWidget.cpp \
    categoryitem.cpp \
    categorypage.cpp \
    favoritepage.cpp \
    homepage.cpp \
    librarypage.cpp \
    main.cpp \
    mainwindow.cpp \
    metadataeditor.cpp \
    playerwidget.cpp \
    settingspage.cpp \
    songitemdelegate.cpp

HEADERS += \
    VolumeWidget.h \
    basepage.h \
    categoryitem.h \
    categorypage.h \
    favoritepage.h \
    homepage.h \
    librarypage.h \
    mainwindow.h \
    metadataeditor.h \
    playerwidget.h \
    settingspage.h \
    songitemdelegate.h

FORMS += \
    VolumeWidget.ui \
    categoryitem.ui \
    categorypage.ui \
    favoritepage.ui \
    homepage.ui \
    librarypage.ui \
    mainwindow.ui \
    metadataeditor.ui \
    playerwidget.ui \
    settingspage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    music-player.qrc
