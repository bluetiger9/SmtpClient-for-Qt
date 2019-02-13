
# Add QT dependencies
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)

if(WIN32)
    ADD_DEFINITIONS( -DSMTP_BUILD )
endif()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(_SRCS
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
    mimecontentformatter.cpp
  )

set( _HEADERS
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
    mimecontentformatter.h
    smtpexports.h
)

set(TARGET_NAME SMTPEmail)

add_library(${TARGET_NAME}
  ${_SRCS}
  ${_HEADERS}
  )

target_link_libraries(${TARGET_NAME} Qt5::Core)
target_link_libraries(${TARGET_NAME} Qt5::Network)

IF(UNIX)
   set_target_properties(${TARGET_NAME} PROPERTIES VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} SOVERSION ${MAJOR_VERSION})
endif()

######################### Installation Stuff ###########################
include(GenerateExportHeader)
generate_export_header(${TARGET_NAME})

install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION ${INSTALL_BIN_DIR}
  LIBRARY DESTINATION ${INSTALL_LIB_DIR}
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
  INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}/${TARGET_NAME}
)

install(
  FILES
    ${_HEADERS}
  DESTINATION
    ${INSTALL_INCLUDE_DIR}/qt5-smtpclient
  COMPONENT
    Devel
)

