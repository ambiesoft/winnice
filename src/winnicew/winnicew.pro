TARGET = winnicew
TEMPLATE = app

SOURCES += \
    winnicew.cpp \
    stdafx.cpp

HEADERS += \
    ../libwinnice/libwinnice.h \
    stdafx.h \
    winnicew.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libwinnice/release/ -llibwinnice
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libwinnice/debug/ -llibwinnice
else:unix: LIBS += -L$$OUT_PWD/../libwinnice/ -llibwinnice

INCLUDEPATH += $$PWD/../libwinnice
DEPENDPATH += $$PWD/../libwinnice


unix {
}

win32 {
   LIBS += -luser32 -lkernel32
}
linux-g++ {
}

