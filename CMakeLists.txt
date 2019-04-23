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

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY Include/ DESTINATION include)

# create and install cmake package files
install(EXPORT ${PROJECT_NAME}Config DESTINATION lib/cmake/${PROJECT_NAME})
#install(EXPORT ${PROJECT_NAME}Config DESTINATION share/${PROJECT_NAME}/cmake)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION lib/cmake/${PROJECT_NAME})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)
#install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


#if(BUILD_TESTS)
#    add_subdirectory(tests)
#endif(BUILD_TESTS)
