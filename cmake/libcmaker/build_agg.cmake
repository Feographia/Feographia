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

set(AGG_lib_NAME      "AGG")
set(AGG_lib_VERSION   "2.4.128")
set(AGG_lib_DIR       "${LibCMaker_libs_DIR}/LibCMaker_${AGG_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${AGG_lib_DIR}/cmake/modules")


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

set(AGG_DIR "${cmr_INSTALL_DIR}")
set(ENV{AGG_DIR} "${AGG_DIR}")
set(AGG_DIR_BIN "${AGG_DIR}/bin")

set(NOT_ADD_AGG_PLATFORM ON)
set(SKIP_BUILD_AGG_EXAMPLES ON)
set(SKIP_BUILD_AGG_MYAPP ON)


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

option(agg_USE_GPC "Use Gpc Boolean library" OFF)
option(agg_USE_FREETYPE "Use Freetype library" OFF)
option(agg_USE_EXPAT "Use Expat library" OFF)
option(agg_USE_SDL_PLATFORM "Use SDL as platform" OFF)
option(agg_USE_PACK "Package Agg" OFF)
option(agg_USE_AGG2D "Agg 2D graphical context" OFF)
option(agg_USE_DEBUG "For debug version" OFF)
option(agg_USE_AGG2D_FREETYPE "Agg 2D graphical context uses freetype" OFF)


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

cmr_find_package(
  LibCMaker_DIR     ${LibCMaker_DIR}
  NAME              ${AGG_lib_NAME}
  VERSION           ${AGG_lib_VERSION}
  LIB_DIR           ${AGG_lib_DIR}
  FIND_MODULE_NAME  Agg
  REQUIRED
  CONFIG
  NOT_USE_VERSION_IN_FIND_PACKAGE
)

#include(${AGG_USE_FILE})
