# ****************************************************************************
#   Project:  Feographia
#   Purpose:  The application to work with the biblical text
#   Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#     Copyright (c) 2017-2020 NikitaFeodonit
#
#     This file is part of the Feographia project.
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published
#     by the Free Software Foundation, either version 3 of the License,
#     or (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#     See the GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program. If not, see <http://www.gnu.org/licenses/>.
# ****************************************************************************

#-----------------------------------------------------------------------
# Set path vars
#-----------------------------------------------------------------------

set(libs_DIR "${PROJECT_SOURCE_DIR}/libs")
set(build_libs_DIR "${PROJECT_BINARY_DIR}/build_libs")
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

if(NOT cmr_DOWNLOAD_DIR)
  set(cmr_DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/.downloads")
endif()

if(NOT cmr_UNPACKED_DIR)
  if(UNIX AND NOT APPLE AND NOT ANDROID)
    set(platform_DIR "_linux")
  elseif(APPLE AND NOT IOS)
    set(platform_DIR "_macos")
  elseif(WIN32)
    set(platform_DIR "_windows")
  elseif(ANDROID)
    set(platform_DIR "_android")
  elseif(IOS)
    set(platform_DIR "_ios")
  endif()

  if(MSVC)
    set(compiler_DIR "_msvc")
  elseif(MINGW)
    set(compiler_DIR "_mingw")
  endif()

  set(cmr_UNPACKED_DIR
    "${cmr_DOWNLOAD_DIR}/.unpacked${platform_DIR}${compiler_DIR}"
  )
endif()

if(NOT cmr_BUILD_DIR)
  set(cmr_BUILD_DIR "${build_libs_DIR}/LibCMaker")
endif()

set(cmr_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")

if(NOT cmr_HOST_UNPACKED_DIR)
  set(cmr_HOST_UNPACKED_DIR "${cmr_UNPACKED_DIR}/.host_tools_sources")
endif()
if(NOT cmr_HOST_BUILD_DIR)
  set(cmr_HOST_BUILD_DIR "${cmr_BUILD_DIR}/.build_host_tools")
endif()


#-----------------------------------------------------------------------
# Configure to find_package()
#-----------------------------------------------------------------------

# Set CMake's search path for find_*() commands.
list(APPEND CMAKE_PREFIX_PATH "${cmr_INSTALL_DIR}")

if(ANDROID OR IOS)
  list(APPEND CMAKE_FIND_ROOT_PATH "${cmr_INSTALL_DIR}")
endif()


#-----------------------------------------------------------------------
# LibCMaker settings
#-----------------------------------------------------------------------

set(LibCMaker_LIB_DIR "${libs_DIR}")
set(LibCMaker_DIR "${LibCMaker_LIB_DIR}/LibCMaker")

list(APPEND CMAKE_MODULE_PATH
  "${LibCMaker_DIR}/cmake"
)

include(cmr_find_package)


#-----------------------------------------------------------------------
# Download, configure, build, install and find the required libraries
#-----------------------------------------------------------------------

# TODO: In LibCMaker:
# 1. Make vars in Find*.cmake files as advanced.
# 2. Print "Found: <lib path>" from all Find*.cmake files.
# 3. Add Libname::Libname targets to all Find*.cmake files.

option(FT_WITH_HARFBUZZ "Improve auto-hinting of OpenType fonts." ON)
# TODO:
#option(FT_WITH_ZLIB "Use system zlib instead of internal library." OFF)
#option(FT_WITH_PNG "Support PNG compressed OpenType embedded bitmaps." OFF)

option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
  enable_testing()
  include(${LibCMaker_LIB_DIR}/LibCMaker_GoogleTest/cmr_build_googletest.cmake)
endif()

# wxWidgets must be 1st, before Cairo and other libs
if(WIN32 OR (UNIX AND NOT ANDROID) OR (APPLE AND NOT IOS))
  set(wxWidgets_PLATFORM ON)
  include(${LibCMaker_LIB_DIR}/LibCMaker_wxWidgets/cmr_build_wxwidgets.cmake)
endif()

find_package(Threads REQUIRED)  # Used in Cairo and ICU

if(MSVC)
  include(${LibCMaker_LIB_DIR}/LibCMaker_Dirent/cmr_build_dirent.cmake)
endif()
include(${LibCMaker_LIB_DIR}/LibCMaker_zlib/cmr_build_zlib.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_libpng/cmr_build_libpng.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_fmt/cmr_build_fmt.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_spdlog/cmr_build_spdlog.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_STLCache/cmr_build_stlcache.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_Expat/cmr_build_expat.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_ICU/cmr_build_icu.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_SQLite3/cmr_build_sqlite3.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_SQLiteModernCPP/cmr_build_sqlitemoderncpp.cmake)
if(APPLE)
  # TODO: Only for iOS <13, macOS <10.15, watchOS <6, tvOS <13 with Xcode 11 or Clang 9
  set(USE_BOOST ON)
  include(${LibCMaker_LIB_DIR}/LibCMaker_Boost/cmr_build_boost.cmake)
endif()
#include(${LibCMaker_LIB_DIR}/LibCMaker_HarfBuzz/cmr_build_harfbuzz.cmake)
include(
  ${LibCMaker_LIB_DIR}/LibCMaker_FreeType/cmr_build_freetype_with_harfbuzz.cmake
)
include(${LibCMaker_LIB_DIR}/LibCMaker_FontConfig/cmr_build_fontconfig.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_Pixman/cmr_build_pixman.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_Cairo/cmr_build_cairo.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_litehtml/cmr_build_litehtml.cmake)
