TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT       -= core gui
CONFIG -= qt

SOURCES += \
    main.cpp

HEADERS += \
    ../libwinnice/libwinnice.h




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libwinnice/release/ -llibwinnice
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libwinnice/debug/ -llibwinnice
else:unix: LIBS += -L$$OUT_PWD/../libwinnice/ -llibwinnice

INCLUDEPATH += $$PWD/../libwinnice
DEPENDPATH += $$PWD/../libwinnice
