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

set(FT_lib_NAME      "FreeType")
set(FT_lib_VERSION   "2.9.1")
set(FT_lib_DIR       "${LibCMaker_libs_DIR}/LibCMaker_${FT_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${FT_lib_DIR}/cmake/modules")


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

# Used in 'cmr_build_rules_harfbuzz.cmake'
set(LIBCMAKER_FREETYPE_SRC_DIR ${FT_lib_DIR})

set(COPY_FREETYPE_CMAKE_BUILD_SCRIPTS ON)


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

set(FREETYPE_NO_DIST ON)

set(FT_WITH_ZLIB OFF)
set(FT_WITH_BZip2 OFF)
set(FT_WITH_PNG OFF)
set(FT_WITH_HarfBuzz ON)

if(FT_WITH_HarfBuzz)
  set(HB_lib_NAME     "HarfBuzz")
  set(HB_lib_VERSION  "1.8.6")
  set(HB_lib_DIR       "${LibCMaker_libs_DIR}/LibCMaker_${HB_lib_NAME}")

  # To use our FindHarfBuzz.cmake.
  list(APPEND CMAKE_MODULE_PATH "${HB_lib_DIR}/cmake/modules")

  # Need in cmr_build_rules_freetype to build HarfBuzz.
  set(LIBCMAKER_HARFBUZZ_SRC_DIR ${HB_lib_DIR})

  # LIBCMAKER_FREETYPE_SRC_DIR needed for lib_cmaker_harfbuzz() to build
  # HarfBuzz with FreeType.
  # LIBCMAKER_FREETYPE_SRC_DIR is set in lib_cmaker_freetype().

  set(COPY_HARFBUZZ_CMAKE_BUILD_SCRIPTS ON)
endif()


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${FT_lib_NAME}
  VERSION         ${FT_lib_VERSION}
  LIB_DIR         ${FT_lib_DIR}
  REQUIRED
  FIND_MODULE_NAME Freetype
  NOT_USE_VERSION_IN_FIND_PACKAGE
  CUSTOM_LOGIC_FILE ${FT_lib_DIR}/cmake/cmr_find_package_freetype_custom.cmake
)
