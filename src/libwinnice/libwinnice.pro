#-------------------------------------------------
#
# Project created by QtCreator 2018-07-07T00:46:51
#
#-------------------------------------------------

QT       -= core gui
CONFIG -= qt

TARGET = libwinnice
TEMPLATE = lib

DEFINES += LIBWINNICE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../../lsMisc/stlsoft/include

SOURCES += \
        libwinnice.cpp \
    ../../../lsMisc/CreateProcessCommon.cpp \
    ../../../lsMisc/GetLastErrorString.cpp \
    ../../../lsMisc/stdwin32/stdwin32.cpp \
    helper.cpp

HEADERS += \
        libwinnice.h \
		libwinnice_global.h \
        ../../../lsMisc/stdosd/SetPrority.h \
	../../../lsMisc/CommandLineString.h \
    mytype.h \
    ../../../lsMisc/CreateProcessCommon.h \
    ../../../lsMisc/GetLastErrorString.h \
    ../../../lsMisc/stdwin32/stdwin32.h \
    helper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
   SOURCES += \
			osd_win32.cpp \
                        ../../../lsMisc/stdosd/SetPrority_win.cpp

   LIBS += -luser32 -lkernel32
}
linux-g++ {
	SOURCES += \
			osd_linux.cpp \
                        ../../../lsMisc/stdosd/SetPrority_linux.cpp
}


