#-------------------------------------------------
#
# Project created by QtCreator 2014-10-30T22:20:42
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = demo3
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    demo3.cpp

# Location of SMTP Library
SMTP_LIBRARY_LOCATION = $$PWD/../../../build/SMTPEmail-Desktop-Debug

win32:CONFIG(release, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/release/ -lSMTPEmail
else:win32:CONFIG(debug, debug|release): LIBS += -L$$SMTP_LIBRARY_LOCATION/debug/ -lSMTPEmail
else:unix: LIBS += -L$$SMTP_LIBRARY_LOCATION -lSMTPEmail

INCLUDEPATH += $$SMTP_LIBRARY_LOCATION
DEPENDPATH += $$SMTP_LIBRARY_LOCATION
