QT += multimedia

lessThan(QT_MAJOR_VERSION, 6): error("The minimum supported version is QT6")

HEADERS += \
    $$PWD/player.h \
    $$PWD/playlist.h

SOURCES += \
    $$PWD/player.cpp \
    $$PWD/playlist.cpp
