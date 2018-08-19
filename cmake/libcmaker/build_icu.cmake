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

set(ICU_lib_NAME        "ICU")
set(ICU_lib_VERSION     "61.1")
set(ICU_lib_COMPONENTS  i18n uc data)
set(ICU_lib_DIR         "${LibCMaker_libs_DIR}/LibCMaker_${ICU_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${ICU_lib_DIR}/cmake/modules")

if(BUILD_FOR_WINXP OR CMAKE_GENERATOR_TOOLSET STREQUAL "v141_xp")
  # This is the last ICU4C release that works on Windows XP and Windows Vista.
  set(ICU_lib_VERSION "58.2")
endif()


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

set(COPY_ICU_CMAKE_BUILD_SCRIPTS ON)


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

# Enable cross compiling
set(ICU_CROSS_COMPILING OFF)
# Specify an absolute path to the build directory of an ICU built for the current platform
set(ICU_CROSS_BUILDROOT "")
if(ICU_CROSS_COMPILING)
  if(NOT cmr_HOST_BUILD_DIR)
    cmr_print_error(
      "Please set cmr_HOST_BUILD_DIR with path to built host tools to cross compilation."
    )
  endif()
  # TODO:
  set(ICU_CROSS_BUILDROOT
    "${cmr_HOST_BUILD_DIR}/build_${ICU_lib_NAME}/icu-${ICU_lib_VERSION}/source"
  )
endif()
# Compile with strict compiler options
set(ICU_ENABLE_STRICT ON)
# Enable auto cleanup of libraries
set(ICU_ENABLE_AUTO_CLEANUP OFF)
# Enable draft APIs (and internal APIs)
set(ICU_ENABLE_DRAFT ON)
# Add a version suffix to symbols
set(ICU_ENABLE_RENAMING ON)
# Enable function and data tracing
set(ICU_ENABLE_TRACING OFF)
# Enable plugins
set(ICU_ENABLE_PLUGINS OFF)
# Disable dynamic loading
set(ICU_DISABLE_DYLOAD OFF)
# Use rpath when linking
set(ICU_ENABLE_RPATH OFF)
# Build ICU extras
set(ICU_ENABLE_EXTRAS OFF) # TODO: not released
# Build ICU's icuio library
set(ICU_ENABLE_ICUIO ON)
# Build ICU's Paragraph Layout library. icu-le-hb must be available via find_package(icu-le-hb). See http://harfbuzz.org
set(ICU_ENABLE_LAYOUTEX OFF) # TODO: not released
# ...
#set(ICU_ENABLE_LAYOUT OFF)
# Build ICU's tools
set(ICU_ENABLE_TOOLS ON)
if(ICU_CROSS_COMPILING)
  set(ICU_ENABLE_TOOLS OFF)
endif()
# Specify how to package ICU data. Possible values: files, archive, library, static, auto. See http://userguide.icu-project.org/icudata for more info
set(ICU_DATA_PACKAGING "auto") # TODO: 'files' mode is not released
# Tag a suffix to the library names
set(ICU_LIBRARY_SUFFIX "")
# Build ICU tests
set(ICU_ENABLE_TESTS OFF) # TODO: not released
# Build ICU samples
set(ICU_ENABLE_SAMPLES OFF) # TODO: not released


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

# TODO: build by COMPONENTS in find_package()

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${ICU_lib_NAME}
  VERSION         ${ICU_lib_VERSION}
  COMPONENTS      ${ICU_lib_COMPONENTS}
  LIB_DIR         ${ICU_lib_DIR}
  REQUIRED
)
