cmake_minimum_required(VERSION 3.5)

project(Qt${QT_VERSION_MAJOR}SmtpMime VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

add_compile_options(-DSMTP_MIME_LIBRARY)

message(USING QT${QT_VERSION_MAJOR})

add_library(${PROJECT_NAME} SHARED
    emailaddress.cpp
    mimeattachment.cpp
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
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Network)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH “${ORIGIN}”)

target_include_directories(${PROJECT_NAME}
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
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/smtpmime
)
