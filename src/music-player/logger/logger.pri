HEADERS += \
    $$PWD/logger.h

SOURCES += \
    $$PWD/logger.cpp

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}
