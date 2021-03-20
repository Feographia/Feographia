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

if(UNIX AND NOT APPLE AND NOT ANDROID)
  set(system_NAME "linux")
elseif(APPLE AND NOT IOS)
  set(system_NAME "macos")
elseif(WIN32)
  set(system_NAME "windows")
elseif(ANDROID)
  set(system_NAME "android")
elseif(IOS)
  set(system_NAME "ios")
endif()

if(MINGW)
  set(system_NAME "${system_NAME}_MinGW")
endif()

set(compiler_NAME "${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}")
if(MSVC)
  set(compiler_NAME
    "${compiler_NAME}_${CMAKE_GENERATOR_PLATFORM}_${CMAKE_GENERATOR_TOOLSET}"
  )
endif()

set(platform_NAME "${system_NAME}_${compiler_NAME}_${CMAKE_BUILD_TYPE}")

set(libs_DIR "${PROJECT_SOURCE_DIR}/libs")
set(build_libs_DIR "${PROJECT_BINARY_DIR}/build_libs")
set(cmr_LIBCMAKER_WORK_DIR "${PROJECT_SOURCE_DIR}/.libcmaker")

if(NOT cmr_DOWNLOAD_DIR)
  set(cmr_DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/.downloads")
endif()

if(NOT cmr_UNPACKED_DIR)
  set(cmr_UNPACKED_DIR
    "${cmr_DOWNLOAD_DIR}/.unpacked${platform_DIR}${compiler_DIR}"
  )
endif()

if(NOT cmr_BUILD_DIR)
  set(cmr_BUILD_DIR "${build_libs_DIR}/LibCMaker")
endif()

if(NOT cmr_HOST_UNPACKED_DIR)
  set(cmr_HOST_UNPACKED_DIR "${cmr_UNPACKED_DIR}/.host_tools_sources")
endif()
if(NOT cmr_HOST_BUILD_DIR)
  set(cmr_HOST_BUILD_DIR "${cmr_BUILD_DIR}/.build_host_tools")
endif()

if(cmr_DEFAULT_CMAKE_INSTALL_PREFIX)
  set(cmr_INSTALL_DIR "${cmr_LIBCMAKER_WORK_DIR}/${platform_NAME}/install")
else()
  set(cmr_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
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

if(ANDROID OR IOS)
  option(ICU_CROSS_COMPILING "Enable cross compiling" ON)
endif()
# TODO: review all options from all libs.
option(BOOST_WITHOUT_ICU "Disable Unicode/ICU support in Regex" OFF)
option(SQLITE_ENABLE_ICU "the ICU extension to SQLite to be added" ON)

option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
  enable_testing()
  include(${LibCMaker_LIB_DIR}/LibCMaker_GoogleTest/cmr_build_googletest.cmake)
endif()

# wxWidgets must be 1st, before Cairo and other libs
if(WIN32 OR (UNIX AND NOT APPLE AND NOT ANDROID) OR (APPLE AND NOT IOS))
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
#if(USE_BOOST_LIB OR ANDROID OR APPLE)
#  # TODO: Android std::filesystem support:
#  # https://developer.android.com/ndk/downloads/revision_history
#  # Android NDK, Revision r22:
#  # std::filesystem support is now included. There are two known issues:
#  #    Issue 1258: std::filesystem::perm_options::nofollow may not be honored on old devices.
#  #    Issue 1260: std::filesystem::canonical will incorrectly succeed when passed a non-existent path on old devices.
#  # Also see:
#  # https://github.com/android/ndk/issues/609
#  # https://android.googlesource.com/platform/ndk/+/master/docs/Roadmap.md#c_file-system-api
#  #
#  # TODO: Apple std::filesystem support:
#  # Only for iOS 13+, macOS 10.15+, watchOS 6+, tvOS 13+ with Xcode 11 or Clang 9
#  # https://developer.apple.com/documentation/xcode_release_notes/xcode_11_release_notes
#  # "Clang now supports the C++17 <filesystem> library for iOS 13, macOS 10.15, watchOS 6, and tvOS 13."
#
#  set(USE_BOOST ON)  # TODO: rename to USE_BOOST_FILESYSTEM
#  set(BOOST_lib_COMPONENTS
#    filesystem system
#    CACHE STRING "BOOST_lib_COMPONENTS"
#  )
#  include(${LibCMaker_LIB_DIR}/LibCMaker_Boost/cmr_build_boost.cmake)
#endif()
include(${LibCMaker_LIB_DIR}/LibCMaker_POCO/cmr_build_poco.cmake)
#include(${LibCMaker_LIB_DIR}/LibCMaker_HarfBuzz/cmr_build_harfbuzz.cmake)
include(
  ${LibCMaker_LIB_DIR}/LibCMaker_FreeType/cmr_build_freetype_with_harfbuzz.cmake
)
include(${LibCMaker_LIB_DIR}/LibCMaker_FontConfig/cmr_build_fontconfig.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_Pixman/cmr_build_pixman.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_Cairo/cmr_build_cairo.cmake)
include(${LibCMaker_LIB_DIR}/LibCMaker_litehtml/cmr_build_litehtml.cmake)
