# ****************************************************************************
#  Project:  HtmlGrapheas
#  Purpose:  HTML text editor library
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2018 NikitaFeodonit
#
#    This file is part of the HtmlGrapheas project.
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

cmake_minimum_required(VERSION 3.3)

# FindThreads work in environments, where only CXX is enabled, since CMake 3.4.
project(FeographiaProject CXX C)

if(WIN32 OR (UNIX AND NOT ANDROID))
  set(wxWidgets_PLATFORM ON)
endif()

set(libs_DIR "${CMAKE_CURRENT_LIST_DIR}/libs")
set(build_libs_DIR "${PROJECT_BINARY_DIR}/build_libs")
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

#set(CMAKE_VERBOSE_MAKEFILE ON)
#set(cmr_PRINT_DEBUG ON)


#-----------------------------------------------------------------------
# Testing
#-----------------------------------------------------------------------

option(BUILD_TESTING "Build the testing tree." OFF)
if(BUILD_TESTING)
  enable_testing()
endif()


#-----------------------------------------------------------------------
# Compiler flags
#-----------------------------------------------------------------------

if(MSVC AND NOT BUILD_SHARED_LIBS)
  set(cmr_USE_MSVC_STATIC_RUNTIME ON)

  # Set MSVC static runtime flags for all configurations.
  # See:
  # https://stackoverflow.com/a/20804336
  # https://stackoverflow.com/a/14172871
  foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
    set(c_flag_var   CMAKE_C_FLAGS)
    set(cxx_flag_var CMAKE_CXX_FLAGS)
    if(cfg)
      string(TOUPPER ${cfg} cfg_upper)
      set(c_flag_var   "${c_flag_var}_${cfg_upper}")
      set(cxx_flag_var "${cxx_flag_var}_${cfg_upper}")
    endif()
    if(${c_flag_var} MATCHES "/MD")
      string(REPLACE "/MD" "/MT" ${c_flag_var} "${${c_flag_var}}")
      set(${c_flag_var} ${${c_flag_var}} CACHE STRING
        "Flags used by the C compiler during ${cfg_upper} builds." FORCE
      )
    endif()
    if(${cxx_flag_var} MATCHES "/MD")
      string(REPLACE "/MD" "/MT" ${cxx_flag_var} "${${cxx_flag_var}}")
      set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
        "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE
      )
    endif()
  endforeach()
endif()

if(NOT DEFINED cmr_BUILD_MULTIPROC)
  set(cmr_BUILD_MULTIPROC ON)
endif()

if(cmr_BUILD_MULTIPROC)
  # Enable /MP flag for Visual Studio 2008 and greater.
  if(MSVC AND MSVC_VERSION GREATER 1400)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${cmr_BUILD_MULTIPROC_CNT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${cmr_BUILD_MULTIPROC_CNT}")
  endif()
endif()


#-----------------------------------------------------------------------
# Configure LibCMaker directories
#-----------------------------------------------------------------------

if(NOT cmr_DOWNLOAD_DIR)
  set(cmr_DOWNLOAD_DIR "${CMAKE_CURRENT_LIST_DIR}/.downloads")
endif()

if(NOT cmr_UNPACKED_DIR)
  if(UNIX AND NOT APPLE AND NOT ANDROID)
    set(platform_DIR "_linux")
  elseif(APPLE)
    set(platform_DIR "_macos")
  elseif(WIN32)
    set(platform_DIR "_windows")
  elseif(ANDROID)
    set(platform_DIR "_android")
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
# Set path to LibCMaker root
#-----------------------------------------------------------------------

set(LibCMaker_DIR "${libs_DIR}/LibCMaker")

list(APPEND CMAKE_MODULE_PATH
#  "${CMAKE_CURRENT_LIST_DIR}/cmake"
  "${CMAKE_CURRENT_LIST_DIR}/cmake/libcmaker"
  "${LibCMaker_DIR}/cmake"
)


#-----------------------------------------------------------------------
# Configure to find_package()
#-----------------------------------------------------------------------

# Set CMake's search path for find_*() commands.
list(APPEND CMAKE_PREFIX_PATH "${cmr_INSTALL_DIR}")

if(ANDROID)
  list(APPEND CMAKE_FIND_ROOT_PATH "${cmr_INSTALL_DIR}")
endif()


#-----------------------------------------------------------------------
# Build, install and find required libraries
#-----------------------------------------------------------------------

# Used by ICU.
find_package(Threads REQUIRED)

include(cmr_find_package)

set(LibCMaker_libs_DIR "${LibCMaker_DIR}/libs")

if(wxWidgets_PLATFORM)
  include(build_wxwidgets)  # Must be 1st, before the other LibCMaker libs.
endif()
if(MSVC)
  include(build_dirent)
endif()
include(build_stlcache)
include(build_expat)
include(build_icu)
include(build_freetype_with_harfbuzz)
include(build_fontconfig)
include(build_agg)


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
