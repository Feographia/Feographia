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
set(BOOST_lib_DIR         "${LibCMaker_libs_DIR}/LibCMaker_${BOOST_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${BOOST_lib_DIR}/cmake/modules")


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

set(BUILD_HOST_TOOLS ON)

#set(PRINT_BOOST_DEBUG ON)  # Extra debug info from 'b2' tool.


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

set(BUILD_BCP_TOOL OFF)  # Build 'bcp' program.

# Disable Unicode/ICU support in Regex.
set(BOOST_WITHOUT_ICU ON)


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${BOOST_lib_NAME}
  VERSION         ${BOOST_lib_VERSION}
  LIB_DIR         ${BOOST_lib_DIR}
  REQUIRED
  FIND_MODULE_NAME BoostHostTools
)
