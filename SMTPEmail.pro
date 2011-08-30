#-------------------------------------------------
#
# Project created by QtCreator 2011-08-11T20:59:25
#
#-------------------------------------------------

QT       += core gui network

TARGET = SMTPEmail
TEMPLATE = app


SOURCES += smtpclient.cpp \
    mimemessage.cpp \
    mimepart.cpp \
    mimetext.cpp \
    mimeattachment.cpp \
    mimehtml.cpp \
    emailaddress.cpp \
    mimefile.cpp \
    mimeinlinefile.cpp \
    main.cpp

HEADERS  += \
    smtpclient.h \
    mimemessage.h \
    mimepart.h \
    mimetext.h \
    mimeattachment.h \
    mimehtml.h \
    emailaddress.h \
    mimefile.h \
    mimeinlinefile.h \
    SmtpMime
