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

# Add this in the project's root CMakeLists.txt.
#option(BUILD_TESTING "Build the testing tree." ON)
#if(BUILD_TESTING)
#  enable_testing()
#endif()


#-----------------------------------------------------------------------
# Lib's name, version, paths
#-----------------------------------------------------------------------

set(GTEST_lib_NAME      "GoogleTest")
set(GTEST_lib_VERSION   "1.8.20180314")
set(GTEST_lib_DIR       "${LibCMaker_libs_DIR}/LibCMaker_${GTEST_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${GTEST_lib_DIR}/cmake/modules")


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Common Google Test and Google Mock options
#
option(BUILD_GTEST "Builds the googletest subproject" ON)
# Note that googlemock target already builds googletest.
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

#-----------------------------------------------------------------------
# Google Test options
#

# When other libraries are using a shared version of runtime libraries,
# Google Test also has to use one.
option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON
)
option(gtest_build_tests "Build all of gtest's own tests." OFF)
option(gtest_build_samples "Build gtest's sample programs." OFF)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)
option(
  gtest_hide_internal_symbols
  "Build gtest with internal symbols hidden in shared libraries."
  OFF
)

#-----------------------------------------------------------------------
# Google Mock options
#
option(gmock_build_tests "Build all of Google Mock's own tests." OFF)


#-----------------------------------------------------------------------
# FindGTest.cmake options
#
# From "FindGTest.cmake":
#   If compiling with MSVC, this variable can be set to ``MT`` or
#   ``MD`` (the default) to enable searching a GTest build tree
if(MSVC)
  set(GTEST_MSVC_SEARCH "MT")
endif()


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${GTEST_lib_NAME}
  VERSION         ${GTEST_lib_VERSION}
  LIB_DIR         ${GTEST_lib_DIR}
  REQUIRED
  FIND_MODULE_NAME GTest
)
