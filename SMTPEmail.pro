#-------------------------------------------------
#
# Project created by QtCreator 2011-08-11T20:59:25
#
#-------------------------------------------------
TARGET = SMTPEmail
TEMPLATE = lib

QT -= gui
QT += core network
DEFINES += SMTP_BUILD

CONFIG(debug, debug|release) {
    win32:      TARGET = $$join(TARGET,,,D)
    mac:        TARGET = $$join(TARGET,,,_debug)
    unix:!mac:  TARGET = $$join(TARGET,,,.debug)
}

SOURCES += \
    Src/emailaddress.cpp \
    Src/mimeattachment.cpp \
    Src/mimefile.cpp \
    Src/mimehtml.cpp \
    Src/mimeinlinefile.cpp \
    Src/mimemessage.cpp \
    Src/mimepart.cpp \
    Src/mimetext.cpp \
    Src/smtpclient.cpp \
    Src/quotedprintable.cpp \
    Src/mimemultipart.cpp \
    Src/mimecontentformatter.cpp \

HEADERS  += \
    Include/SMTPEmail/emailaddress.h \
    Include/SMTPEmail/mimeattachment.h \
    Include/SMTPEmail/mimefile.h \
    Include/SMTPEmail/mimehtml.h \
    Include/SMTPEmail/mimeinlinefile.h \
    Include/SMTPEmail/mimemessage.h \
    Include/SMTPEmail/mimepart.h \
    Include/SMTPEmail/mimetext.h \
    Include/SMTPEmail/smtpclient.h \
    Include/SMTPEmail/SmtpMime \
    Include/SMTPEmail/quotedprintable.h \
    Include/SMTPEmail/mimemultipart.h \
    Include/SMTPEmail/mimecontentformatter.h \
    Include/SMTPEmail/smtpexports.h

OTHER_FILES += \
    LICENSE \
    README.md


INCLUDEPATH += Include/SMTPEmail

unix {
    target.path = /usr/local/lib
    copy_lib_headers.path = /usr/local/include/SMTPEmail/
    copy_lib_headers.files = $$PWD/Include/SMTPEmail/*.h
    INSTALLS += target copy_lib_headers
}
