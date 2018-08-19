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

#-----------------------------------------------------------------------
# Lib's name, version, paths
#-----------------------------------------------------------------------

set(STLCACHE_lib_NAME      "STLCache")
set(STLCACHE_lib_VERSION   "0.2.20180405")
set(STLCACHE_lib_DIR     "${LibCMaker_libs_DIR}/LibCMaker_${STLCACHE_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${STLCACHE_lib_DIR}/cmake/modules")


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

set(COPY_STLCACHE_CMAKE_BUILD_SCRIPTS ON)


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${STLCACHE_lib_NAME}
  VERSION         ${STLCACHE_lib_VERSION}
  LIB_DIR         ${STLCACHE_lib_DIR}
  REQUIRED
)
