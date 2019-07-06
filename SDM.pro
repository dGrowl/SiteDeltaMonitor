#-------------------------------------------------
#
# Project created by QtCreator 2019-05-27T14:43:23
#
#-------------------------------------------------

QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = SDM
TEMPLATE = app
CONFIG  += c++14
DEFINES += QT_DEPRECATED_WARNINGS
LIBS    += -lgumbo

QMAKE_CXXFLAGS         += -pedantic
QMAKE_CXXFLAGS_RELEASE += -Os
QMAKE_LFLAGS_RELEASE   += -s

SOURCES += \
        src/check.cpp \
        src/main.cpp \
        src/icon.cpp \
        src/profilewindow.cpp \
        src/reportwindow.cpp

HEADERS += \
    inc/check.h \
    inc/icon.h \
    inc/profilewindow.h \
    inc/reportwindow.h

FORMS += \
    src/profilewindow.ui \
    src/reportwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
