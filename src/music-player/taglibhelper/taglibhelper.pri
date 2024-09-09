HEADERS += \
    $$PWD/taglibhelper.h

SOURCES += \
    $$PWD/taglibhelper.cpp

win32 {
    LIBS += user32.lib
    LIBS += -L$$PWD/taglib/lib/ -ltag

    INCLUDEPATH += $$PWD/taglib/include
    DEPENDPATH += $$PWD/taglib/include
}
