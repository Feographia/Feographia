# ****************************************************************************
#  Project:  Feographia
#  Purpose:  The application to work with the biblical text
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2020 NikitaFeodonit
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

project(Feographia CXX)

option(BUILD_SHARED_LIBS "BUILD_SHARED_LIBS" OFF)

option(CMAKE_VERBOSE_MAKEFILE "CMAKE_VERBOSE_MAKEFILE" OFF)
option(cmr_PRINT_DEBUG "cmr_PRINT_DEBUG" OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(compile_flags)
include(find_libraries)
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
