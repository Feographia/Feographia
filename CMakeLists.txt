# ****************************************************************************
#  Project:  Feographia
#  Purpose:  The application to work with the biblical text
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2018 NikitaFeodonit
#
#    This file is part of the Feographia project.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published
#    by the Free Software Foundation, either version 3 of the License,
#    or (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#    See the GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program. If not, see <http://www.gnu.org/licenses/>.
# ****************************************************************************

set(prj_options "${CMAKE_CURRENT_LIST_DIR}/.prj_options.cmake")
if(EXISTS ${prj_options})
  include(${prj_options})
endif()

cmake_minimum_required(VERSION 3.8)
# C++17 compiler:
#   GCC 8
#   Clang 8 (Android NDK r19)
#   Apple Clang 11.0.0, Xcode 11.0
#   MSVC 19.15, VS 2017 15.8
# See: https://en.cppreference.com/w/cpp/compiler_support

project(FeographiaProject CXX)

option(BUILD_SHARED_LIBS "BUILD_SHARED_LIBS" OFF)

option(CMAKE_VERBOSE_MAKEFILE "CMAKE_VERBOSE_MAKEFILE" OFF)
option(cmr_PRINT_DEBUG "cmr_PRINT_DEBUG" OFF)


#-----------------------------------------------------------------------
# Compiler flags
#-----------------------------------------------------------------------

# Used by LibCMaker_Boost.
set(CMAKE_CXX_STANDARD 17)

if(NOT BUILD_SHARED_LIBS)
  set(cmr_USE_STATIC_RUNTIME ON)
endif()

# Set compile flags.
if(MSVC)
  # Determine MSVC runtime library flag
  set(MSVC_LIB_USE "/MD")
  set(MSVC_LIB_REPLACE "/MT")
  if(cmr_USE_STATIC_RUNTIME AND NOT BUILD_SHARED_LIBS)
    set(MSVC_LIB_USE "/MT")
    set(MSVC_LIB_REPLACE "/MD")
  endif()
  # Set MSVC runtime flags for all configurations
  # See:
  # https://stackoverflow.com/a/20804336
  # https://stackoverflow.com/a/14172871
  foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
    set(c_flag_var CMAKE_C_FLAGS)
    set(cxx_flag_var CMAKE_CXX_FLAGS)
    if(cfg)
      string(TOUPPER ${cfg} cfg_upper)
      set(c_flag_var   "${c_flag_var}_${cfg_upper}")
      set(cxx_flag_var "${cxx_flag_var}_${cfg_upper}")
    endif()
    if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
      string(REPLACE
        ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}"
      )
      set(${c_flag_var} ${${c_flag_var}} CACHE STRING
        "Flags used by the C compiler during ${cfg_upper} builds." FORCE
      )
    endif()
    if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
      string(REPLACE
        ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}"
      )
      set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
        "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE
      )
    endif()
  endforeach()

elseif(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
  if(cmr_USE_STATIC_RUNTIME AND NOT BUILD_SHARED_LIBS)
    if(MINGW)
      set(STATIC_LINKER_FLAGS "-static")
    elseif(NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND APPLE))
      set(STATIC_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} ${STATIC_LINKER_FLAGS}"
    )
    set(CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS} ${STATIC_LINKER_FLAGS}"
    )
  endif()
endif()

option(cmr_BUILD_MULTIPROC "cmr_BUILD_MULTIPROC" ON)

if(cmr_BUILD_MULTIPROC)
  # Enable /MP flag for Visual Studio 2008 and greater.
  if(MSVC AND MSVC_VERSION GREATER 1400)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${cmr_BUILD_MULTIPROC_CNT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${cmr_BUILD_MULTIPROC_CNT}")
  endif()
endif()


#-----------------------------------------------------------------------
# Set path vars
#-----------------------------------------------------------------------

set(libs_DIR "${CMAKE_CURRENT_LIST_DIR}/libs")
set(build_libs_DIR "${PROJECT_BINARY_DIR}/build_libs")
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

if(NOT cmr_DOWNLOAD_DIR)
  set(cmr_DOWNLOAD_DIR "${CMAKE_CURRENT_LIST_DIR}/.downloads")
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
#  "${CMAKE_CURRENT_LIST_DIR}/cmake"
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


#[==[  # TODO: Use litehtml for viewing of dictionaries and other HTML texts.
#-----------------------------------------------------------------------
# LiteHtml
#-----------------------------------------------------------------------

if(WIN32 OR WINCE)
  set(LITEHTML_UTF8 "LITEHTML_UTF8")
  add_definitions(-D${LITEHTML_UTF8})
endif()

set(LITEHTML_SRC_DIR      "${libs_DIR}/litehtml")
set(LITEHTML_BUILD_DIR    "${build_libs_DIR}/litehtml")
set(LITEHTML_EXPORT_FILE  "${LITEHTML_BUILD_DIR}/export-litehtml.cmake")

if(NOT EXISTS ${LITEHTML_EXPORT_FILE})
  cmr_print_status("litehtml is not built, build it.")

  add_subdirectory(${LITEHTML_SRC_DIR} ${LITEHTML_BUILD_DIR})

  export(
    TARGETS gumbo litehtml
    FILE ${LITEHTML_EXPORT_FILE}
    EXPORT_LINK_INTERFACE_LIBRARIES
  )

else()
  cmr_print_status("litehtml is built, skip its building.")
  include(${LITEHTML_EXPORT_FILE})
endif()


#-----------------------------------------------------------------------
# HtmlGrapheas
#-----------------------------------------------------------------------

set(HGraph_DIR "${libs_DIR}/HtmlGrapheas")
add_subdirectory(${HGraph_DIR} "${build_libs_DIR}/HtmlGrapheas")


#-----------------------------------------------------------------------
# HtmlGrapheasKamva
#-----------------------------------------------------------------------

set(HGKamva_DIR "${libs_DIR}/HtmlGrapheasKamva")
add_subdirectory("${HGKamva_DIR}" "${build_libs_DIR}/HtmlGrapheasKamva")
]==]


#-----------------------------------------------------------------------
# FgKardia
#-----------------------------------------------------------------------

set(FgKardia_DIR "${libs_DIR}/FgKardia")
#add_subdirectory("${FgKardia_DIR}" "${build_libs_DIR}/FgKardia")


#-----------------------------------------------------------------------
# Platforms
#-----------------------------------------------------------------------


#-----------------------------------------------------------------------
# wxWidgets
#-----------------------------------------------------------------------

# To run the application on Linux with the shared libs which are installed
# to the non standard path specified in CMAKE_INSTALL_PREFIX use:
# LD_LIBRARY_PATH="<${CMAKE_INSTALL_PREFIX}>/lib:.:$LD_LIBRARY_PATH" ./html_grapheas
# where <${CMAKE_INSTALL_PREFIX}> is path which is used
# as value for CMAKE_INSTALL_PREFIX.

if(wxWidgets_PLATFORM)
  # HtmlGrapheasKamvaWx
  set(HGKamvaWx_DIR "${HGKamva_DIR}/platform/wxwidgets")
  add_subdirectory("${HGKamvaWx_DIR}" "${build_libs_DIR}/HtmlGrapheasKamvaWx")

  # FeographiaAppWx
  set(FgAppWx_DIR ${CMAKE_CURRENT_LIST_DIR}/platform/wxwidgets)
  add_subdirectory("${FgAppWx_DIR}" "${PROJECT_BINARY_DIR}/FeographiaAppWx")
endif()


#-----------------------------------------------------------------------
# Android
#-----------------------------------------------------------------------

if(ANDROID)
  # HtmlGrapheasKamvaAndroid
  set(HGKamvaAndroid_DIR ${HGKamva_DIR}/platform/android/main/cpp)
  add_subdirectory(
    "${HGKamvaAndroid_DIR}" "${build_libs_DIR}/HtmlGrapheasKamvaAndroid"
  )
endif()
