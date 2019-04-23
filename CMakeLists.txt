cmake_minimum_required (VERSION 3.0)

project (SMTPEmail VERSION 1.1.0 LANGUAGES CXX)

# set options
option(STATIC_LIB "build as static lib if ON, otherwise build shared lib" OFF)
option(USE_QT4 "builds against Qt4 if ON, otherwise builds against Qt5" OFF)
#option(BUILD_TESTS "build tests" OFF)

# settings
set(BUILD_LIBRARY_DEFINITION SMTP_BUILD)

# find qt package
if(USE_QT4)
    find_package(Qt4Core REQUIRED)
    find_package(Qt4Network REQUIRED)
    set(QT_TARGET Qt4::QtCore Qt4::Network)
else()
    # if cmake failed to find Qt5Core configuration file, set path manually:
    #list(APPEND CMAKE_PREFIX_PATH "/path/to/Qt/lib/cmake/Qt5Core/")

    find_package(Qt5Core REQUIRED)
    find_package(Qt5Network REQUIRED)
    set(QT_TARGET Qt5::Core Qt5::Network)
endif(USE_QT4)

set(CMAKE_DEBUG_POSTFIX .debug)

# instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# set list of source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cxx ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c++)
#message("Source files:" ${SOURCE_FILES})

file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h)
#message("HEADER_FILES: " ${HEADER_FILES})

# Show all files in QtCreator. Starting with CMake 3.7 server-mode is used
# and QtCreator will show the files properly in an extra <Headers> section.
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    file(GLOB_RECURSE ALL_FILES "*")
    add_custom_target(show_all_files_in_${PROJECT_NAME} SOURCES ${ALL_FILES})
endif()

if(STATIC_LIB)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
    #propogate the STATIC_LIB define for all consuming stuff
    #necessary for correct ${PROJECT_NAME}SHARED_EXPORT handling, espacially for windows (cross compilation) builds
    target_compile_definitions(${PROJECT_NAME} PUBLIC -D${PROJECT_NAME}_STATIC_LIB)
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})
endif(STATIC_LIB)

target_compile_definitions(${PROJECT_NAME} PRIVATE -D${BUILD_LIBRARY_DEFINITION})

# include root project folder as private, because the source headers are included with source/*.h
target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/${PROJECT_NAME}> $<INSTALL_INTERFACE:include> PRIVATE .)

# set compiler flags for the library target
if (CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wformat=2
        -Wuninitialized -Winit-self -Wswitch-enum -Wundef
        -Wpointer-arith -Wdisabled-optimization -Wcast-align -Wcast-qual)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_TARGET})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

# Install
include(GNUInstallDirs)

# Install headers
install(DIRECTORY Include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install exe, library, and archive files - we can't install headers here because we can have directory structure (not just flat subdir with headers)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE})

# Create and install Cmake Config file
install(EXPORT ${PROJECT_NAME}Config DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/${PROJECT_NAME})

# Create and install Cmake version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file( ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/${PROJECT_NAME})

# Create and install pc file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CMAKE_LIBRARY_ARCHITECTURE}/pkgconfig)

#if(BUILD_TESTS)
#    add_subdirectory(tests)
#endif(BUILD_TESTS)
