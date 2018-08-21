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

#-----------------------------------------------------------------------
# Lib's name, version, paths
#-----------------------------------------------------------------------

set(BOOST_lib_NAME        "Boost")
set(BOOST_lib_VERSION     "1.68.0")
set(BOOST_lib_COMPONENTS  regex)  # deps will also be compiled.
#set(BOOST_lib_COMPONENTS  all)
set(BOOST_lib_DIR         "${LibCMaker_libs_DIR}/LibCMaker_${BOOST_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${BOOST_lib_DIR}/cmake/modules")

# Set required compiler language standards.
# Set in main project.
#set(CMAKE_C_STANDARD 99)    # 11 99 90
#set(CMAKE_CXX_STANDARD 11)  # 17 14 11 98


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

#set(PRINT_BOOST_DEBUG ON)  # Extra debug info from 'b2' tool.


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

set(b2_FILE_NAME "b2")
if(CMAKE_HOST_WIN32)
  set(b2_FILE_NAME "b2.exe")
endif()
set(_b2_program_path "${cmr_INSTALL_DIR}/bin/${b2_FILE_NAME}")

if(EXISTS ${_b2_program_path})
  set(B2_PROGRAM_PATH ${_b2_program_path})  # Use 'b2' in specified path.
endif()

set(BUILD_BCP_TOOL OFF)  # Build 'bcp' program.
set(Boost_USE_MULTITHREADED ON)  # Use to build lib and for find_project().

# Whether to link to static or shared C and C++ runtime.
# If DEFINED then add 'runtime-link' to 'b2' tool with value
# 'static' for 'ON' or 'shared' for OFF.
if(BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_RUNTIME OFF)
else()
  set(Boost_USE_STATIC_RUNTIME ON)
endif()

# Determine whether to choose library names and header locations.
set(BOOST_LAYOUT_TYPE "tagged")  # "versioned", "tagged" or "system",
# From 'b2 --help':
# Determine whether to choose library names and header locations
# such that multiple versions of Boost or multiple compilers
# can be used on the same system.
# -- versioned -- Names of boost binaries include the Boost version number,
# name and version of the compiler and encoded build properties.
# Boost headers are installed in a subdirectory of <HDRDIR>
# whose name contains the Boost version number.
# -- tagged -- Names of boost binaries include the encoded build properties
# such as variant and threading, but do not including compiler name and
# version, or Boost version. This option is useful if you build several
# variants of Boost, using the same compiler.
# -- system -- Binaries names do not include the Boost version number or
# the name and version number of the compiler. Boost headers are installed
# directly into <HDRDIR>. This option is intended for system integrators
# building distribution packages.

# Disable Unicode/ICU support in Regex.
set(BOOST_WITHOUT_ICU OFF)

# Specify the root of the ICU library installation
# and enable Unicode/ICU support in Regex.
#set(BOOST_WITH_ICU_DIR "DIR")  # See setting below.

# Build and install only compiled library files to the stage directory.
set(BOOST_BUILD_STAGE OFF)
# Install library files here.
set(BOOST_BUILD_STAGE_DIR "${cmr_INSTALL_DIR}/stage")

if(NOT BOOST_WITHOUT_ICU)
  set(BOOST_WITH_ICU_DIR "${cmr_INSTALL_DIR}")  # See description above.
endif()


#-----------------------------------------------------------------------
# Vars to find_project(Boost) only
#-----------------------------------------------------------------------

# Use to select installed Boost configuration.
# For more vars see in 'cmake/FindBoost.cmake' file.

#set(Boost_USE_MULTITHREADED ON)  # Set above.
#set(Boost_USE_STATIC_RUNTIME ON)  # Set above.

# Here we depend to BUILD_SHARED_LIBS for Boost_USE_STATIC_LIBS.
if(BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
else()
  set(Boost_USE_STATIC_LIBS ON)
endif()

#set(BOOST_ROOT ${cmr_INSTALL_DIR})
set(Boost_NO_SYSTEM_PATHS ON)


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

# For specified boost components dependence components will builded too.

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${BOOST_lib_NAME}
  VERSION         ${BOOST_lib_VERSION}
  COMPONENTS      ${BOOST_lib_COMPONENTS}
  LIB_DIR         ${BOOST_lib_DIR}
  REQUIRED
)
