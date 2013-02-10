#-------------------------------------------------
#
# Project created by QtCreator 2012-09-22T16:39:45
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    connectiontest.cpp

HEADERS += \
    connectiontest.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/lib/release/ -lSmtpMime
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/lib/debug/ -lSmtpMime
else:unix:!symbian: LIBS += -L$$PWD/../bin/lib/release/ -lSmtpMime

INCLUDEPATH += $$PWD/../bin/lib/release
DEPENDPATH += $$PWD/../bin/lib/release
