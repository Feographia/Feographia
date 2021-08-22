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

set(prj_options "${CMAKE_CURRENT_LIST_DIR}/.prj_options.cmake")
if(EXISTS ${prj_options})
  include(${prj_options})
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  option(cmr_DEFAULT_CMAKE_INSTALL_PREFIX "cmr_DEFAULT_CMAKE_INSTALL_PREFIX" ON)
endif()

cmake_minimum_required(VERSION 3.9)
# C++17 compiler:
#   GCC 8
#   Clang 8 (Android NDK r19)
#   Apple Clang 11.0.0, Xcode 11.0
#   MSVC 19.15, VS 2017 15.8
# See: https://en.cppreference.com/w/cpp/compiler_support

# If LTO is used, all these languages must be enabled.
project(Feographia CXX C ASM)

include(CheckIPOSupported)
check_ipo_supported(RESULT is_ipo_supported)
if(is_ipo_supported)
  # NOTE: Turn OFF ccache if there are problems with linking and LTO.
  option(CMAKE_INTERPROCEDURAL_OPTIMIZATION
    "CMAKE_INTERPROCEDURAL_OPTIMIZATION" OFF
  )
endif()

option(USE_BOOST_LIB "USE_BOOST_LIB" OFF)

option(BUILD_SHARED_LIBS "BUILD_SHARED_LIBS" OFF)

option(CMAKE_VERBOSE_MAKEFILE "CMAKE_VERBOSE_MAKEFILE" OFF)
option(cmr_PRINT_DEBUG "cmr_PRINT_DEBUG" OFF)


#-----------------------------------------------------------------------
# Check compiler's version
#-----------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0.0)
  message(FATAL_ERROR "GNU g++ compiler version 9+ is required.")
endif()

if((CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
     OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
     AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0.0)
  message(FATAL_ERROR "Clang compiler version 9+ is required.")
endif()

if(MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.15)
  message(FATAL_ERROR "MSVC compiler version 19.15+ is required.")
endif()


#-----------------------------------------------------------------------
# Feographia project
#-----------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(print_system_info)
print_system_info()

include(target_settings)
include(compile_flags)

# For Travis CI time limits.
set(TRAVIS_BUILD_STAGE_NAME $ENV{TRAVIS_BUILD_STAGE_NAME})
include(find_libraries)
if(TRAVIS_BUILD_STAGE_NAME MATCHES "Configure project stage")
  message(STATUS "Exit from Travis CI stage \"${TRAVIS_BUILD_STAGE_NAME}\"")
  return()
endif()

# TODO: Use litehtml for viewing of dictionaries and other HTML texts.
#include(htmlgrapheas)


#-----------------------------------------------------------------------
# Feographia sources
#-----------------------------------------------------------------------

add_subdirectory(
  "${CMAKE_CURRENT_LIST_DIR}/src"
  "${PROJECT_BINARY_DIR}/Feographia"
)

add_subdirectory(test)
