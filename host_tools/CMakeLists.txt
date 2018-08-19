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
project(HtmlGrapheasKamvaHostTools CXX C)
set(lib_NAME "hgkamva_host_tools")

if(NOT HGMainProject_DIR)
  message(FATAL_ERROR
    "Please set HGMainProject_DIR with path to main project root directory.")
endif()
if(NOT LibCMaker_DIR)
  message(FATAL_ERROR
    "Please set LibCMaker_DIR with path to LibCMaker root directory.")
endif()
if(NOT HGKamva_DIR)
  message(FATAL_ERROR
    "Please set HGKamva_DIR with path to HtmlGrapheasKamva library.")
endif()
if(NOT HOST_TOOLS_STAMP_FILE_NAME)
  message(FATAL_ERROR
    "Please set HOST_TOOLS_STAMP_FILE_NAME with name of stamp file.")
endif()


#-----------------------------------------------------------------------
# Configure external libs directories
#-----------------------------------------------------------------------

if(NOT cmr_DOWNLOAD_DIR)
  set(cmr_DOWNLOAD_DIR "${CMAKE_CURRENT_LIST_DIR}/.downloads")
endif()
if(NOT cmr_UNPACKED_DIR)
  set(cmr_UNPACKED_DIR "${PROJECT_BINARY_DIR}/host_tools/sources")
endif()
if(NOT cmr_BUILD_DIR)
  set(cmr_BUILD_DIR "${PROJECT_BINARY_DIR}/host_tools/build")
endif()

set(cmr_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")


#-----------------------------------------------------------------------
# Set path to LibCMaker root
#-----------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH
  "${HGMainProject_DIR}/cmake/libcmaker"
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
# Set vars to HtmlGrapheasKamvaHostTools
#-----------------------------------------------------------------------


#-----------------------------------------------------------------------
# Build, install and find required libraries
#-----------------------------------------------------------------------

# Used by ICU.
find_package(Threads REQUIRED)

include(cmr_find_package)

set(LibCMaker_libs_DIR "${LibCMaker_DIR}/libs")

include(build_icu_host_tools)


#-----------------------------------------------------------------------
# HtmlGrapheasKamvaHostTools
#-----------------------------------------------------------------------

add_custom_target(${lib_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E touch
    "${PROJECT_BINARY_DIR}/${HOST_TOOLS_STAMP_FILE_NAME}"
  COMMAND ${CMAKE_COMMAND} -E echo
    "Host tools are ready."
)
