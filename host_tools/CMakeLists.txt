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

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(cmr_DEFAULT_CMAKE_INSTALL_PREFIX ON)
endif()

cmake_minimum_required(VERSION 3.9)

project(FeographiaHostTools CXX)
set(lib_NAME "feographia_host_tools")

option(CMAKE_VERBOSE_MAKEFILE "CMAKE_VERBOSE_MAKEFILE" OFF)
option(cmr_PRINT_DEBUG "cmr_PRINT_DEBUG" OFF)


#-----------------------------------------------------------------------
# Set path vars
#-----------------------------------------------------------------------

if(NOT FgMainProject_DIR)
  message(FATAL_ERROR
    "Please set FgMainProject_DIR with path to main project root directory.")
endif()
#[==[  # TODO: Use litehtml for viewing of dictionaries and other HTML texts.
if(NOT HGKamva_DIR)
  message(FATAL_ERROR
    "Please set HGKamva_DIR with path to HtmlGrapheasKamva library.")
endif()
]==]
if(NOT LibCMaker_LIB_DIR)
  message(FATAL_ERROR
    "Please set LibCMaker_LIB_DIR with path to LibCMaker's libraries directory.")
endif()
if(NOT HOST_TOOLS_STAMP_FILE_NAME)
  message(FATAL_ERROR
    "Please set HOST_TOOLS_STAMP_FILE_NAME with name of stamp file.")
endif()

if(UNIX AND NOT APPLE AND NOT ANDROID)
  set(platform_NAME "linux")
elseif(APPLE AND NOT IOS)
  set(platform_NAME "macos")
elseif(WIN32)
  set(platform_NAME "windows")
elseif(ANDROID)
  set(platform_NAME "android")
elseif(IOS)
  set(platform_NAME "ios")
endif()

set(compiler_NAME "${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}")
if(MINGW)
  set(compiler_NAME "MinGW_${compiler_NAME}")
endif()

set(platform_NAME "${platform_NAME}_${compiler_NAME}")
set(cmr_LIBCMAKER_WORK_DIR "${PROJECT_SOURCE_DIR}/.libcmaker")

if(NOT cmr_DOWNLOAD_DIR)
  set(cmr_DOWNLOAD_DIR "${CMAKE_CURRENT_LIST_DIR}/.downloads")
endif()
if(NOT cmr_UNPACKED_DIR)
  set(cmr_UNPACKED_DIR "${PROJECT_BINARY_DIR}/host_tools/sources")
endif()
if(NOT cmr_BUILD_DIR)
  set(cmr_BUILD_DIR "${PROJECT_BINARY_DIR}")
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

set(LibCMaker_DIR "${LibCMaker_LIB_DIR}/LibCMaker")
list(APPEND CMAKE_MODULE_PATH "${LibCMaker_DIR}/cmake")
include(cmr_find_package)


#-----------------------------------------------------------------------
# Download, configure, build, install and find the required libraries
#-----------------------------------------------------------------------

# Used in ICU.
find_package(Threads REQUIRED)

include(${LibCMaker_LIB_DIR}/LibCMaker_ICU/cmr_build_icu_host_tools.cmake)
# NOTE: See notes in 'cmake/find_libraries.cmake' in the main project.
#include(${LibCMaker_LIB_DIR}/LibCMaker_Boost/cmr_build_boost_host_tools.cmake)


#-----------------------------------------------------------------------
# Stub target
#-----------------------------------------------------------------------

add_custom_target(${lib_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E touch
    "${PROJECT_BINARY_DIR}/${HOST_TOOLS_STAMP_FILE_NAME}"
  COMMAND ${CMAKE_COMMAND} -E echo
    "Host tools are ready."
)
