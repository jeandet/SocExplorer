cmake_minimum_required(VERSION 3.6)
project(SocExplorer CXX)

include(GNUInstallDirs)

OPTION (CPPCHECK "Analyzes the source code with cppcheck" OFF)
OPTION (CLANG_TIDY "Analyzes the source code with Clang Tidy" OFF)
OPTION (IWYU "Analyzes the source code with Include What You Use" OFF)

option(TeamCity "Change unit tests output format to TeamCity" OFF)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
#https://gitlab.kitware.com/cmake/cmake/issues/15227
#set(CMAKE_AUTOUIC ON)
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 OLD)
endif()
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT DEFINED CMAKE_INSTALL_RPATH_USE_LINK_PATH)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH TRUE)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

find_package(Qt5 COMPONENTS Core Widgets Multimedia Network PrintSupport Svg Test Xml REQUIRED)

IF(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK "cppcheck;--enable=warning,style")
ENDIF(CPPCHECK)

IF(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-style=file;-checks=*")
ENDIF(CLANG_TIDY)

IF(IWYU)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use")
ENDIF(IWYU)

enable_testing()

add_definitions(-DSOCEXPLORER_VERSION="0.8")
add_definitions(-DSOCEXPLORER_CHAGESET="nope")
add_definitions(-DSOCEXPLORER_BRANCH="cmake")
add_definitions(-DUNIX)
add_definitions(-DSOCEXPLORER_TRANSLATION_PATH="/etc/SocExplorer/translations")
add_definitions(-DSOCEXPLORER_CONFIG_PATH="/etc/SocExplorer")
add_definitions(-DSOCEXPLORER_SHARE_PATH="/usr/share/SocExplorer")

add_subdirectory(opaque-1.3.0)
add_subdirectory(src)
add_subdirectory(tests)

