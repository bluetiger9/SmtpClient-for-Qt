add_compile_options(-DSMTP_MIME_LIBRARY)

qt_add_library(${LIBRARY_TARGET_NAME}
    emailaddress.cpp
    mimeattachment.cpp
    mimebytearrayattachment.cpp
    mimefile.cpp
    mimehtml.cpp
    mimeinlinefile.cpp
    mimemessage.cpp
    mimepart.cpp
    mimetext.cpp
    smtpclient.cpp
    quotedprintable.cpp
    mimemultipart.cpp
    mimecontentencoder.cpp
    mimebase64encoder.cpp
    mimeqpencoder.cpp
    mimeqpformatter.cpp
    mimebase64formatter.cpp
    mimecontentformatter.cpp
    emailaddress.h
    mimeattachment.h
    mimebytearrayattachment.h
    mimefile.h
    mimehtml.h
    mimeinlinefile.h
    mimemessage.h
    mimepart.h
    mimetext.h
    smtpclient.h
    SmtpMime
    quotedprintable.h
    mimemultipart.h
    smtpmime_global.h
    mimecontentencoder.h
    mimebase64encoder.h
    mimeqpencoder.h
    mimeqpformatter.h
    mimebase64formatter.h
    mimecontentformatter.h
)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(${LIBRARY_TARGET_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/smtpmime>
)

set(public_headers
    SmtpMime
    emailaddress.h
    mimeattachment.h
    mimefile.h
    mimehtml.h
    mimeinlinefile.h
    mimemessage.h
    mimepart.h
    mimetext.h
    smtpclient.h
    SmtpMime
    quotedprintable.h
    mimemultipart.h
    smtpmime_global.h
    mimecontentencoder.h
    mimebase64encoder.h
    mimeqpencoder.h
    mimeqpformatter.h
    mimebase64formatter.h
    mimecontentformatter.h
    mimebytearrayattachment.h
)
# note that ${public_headers} has to be in quotes
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION_MAJOR}
	PUBLIC_HEADER "${public_headers}"
)

include(GNUInstallDirs)
install(TARGETS ${LIBRARY_TARGET_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/smtpmime
)

