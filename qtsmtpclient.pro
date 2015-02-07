QT       += core network
TARGET = qtsmtpclient

TEMPLATE = lib
CONFIG += staticlib

DEFINES += SMTP_BUILD

QMAKE_CXXFLAGS += -fPIC

SOURCES += \
    emailaddress.cpp \
    mimeattachment.cpp \
    mimefile.cpp \
    mimehtml.cpp \
    mimeinlinefile.cpp \
    mimemessage.cpp \
    mimepart.cpp \
    mimetext.cpp \
    smtpclient.cpp \
    quotedprintable.cpp \
    mimemultipart.cpp \
    mimecontentformatter.cpp \

HEADERS  += \
    emailaddress.h \
    mimeattachment.h \
    mimefile.h \
    mimehtml.h \
    mimeinlinefile.h \
    mimemessage.h \
    mimepart.h \
    mimetext.h \
    smtpclient.h \
    SmtpMime \
    quotedprintable.h \
    mimemultipart.h \
    mimecontentformatter.h \
    smtpexports.h

OTHER_FILES += \
    LICENSE \
    README.md

FORMS +=
