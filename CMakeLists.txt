cmake_minimum_required(VERSION 3.21)

project(SmtpMime
  VERSION 2.0
  LANGUAGES C CXX)

option(BUILD_TESTS "builds tests" ON)
option(BUILD_DEMOS "builds demos" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)

find_package(QT NAMES Qt6 COMPONENTS Core Network Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Test REQUIRED)

set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

message("Using Qt" ${QT_VERSION_MAJOR})

#
# Allow -DBUILD_SHARED_LIBS to control the library type built by qt_add_library
#
qt_policy(SET QTP0003 NEW)

#
# Set the default for BUILD_SHARED_LIBS to ON, but allow command line
# to override using -DBUILD_SHARED_LIBS=OFF
#
option(BUILD_SHARED_LIBS "Build as a shared library" ON)

set(CMAKE_INSTALL_RPATH $ORIGIN)

if(BUILD_SHARED_LIBS)
  add_compile_options(-DBUILD_SHARED_LIBS)
endif()

add_subdirectory(src)

if (BUILD_TESTS)
  add_subdirectory(test)
endif()

if (BUILD_DEMOS)
  add_subdirectory(demos)
endif()
