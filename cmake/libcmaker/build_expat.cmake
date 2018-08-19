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

set(EXPAT_lib_NAME      "Expat")
set(EXPAT_lib_VERSION   "2.2.5")
set(EXPAT_lib_DIR       "${LibCMaker_libs_DIR}/LibCMaker_${EXPAT_lib_NAME}")

# To use our Find<LibName>.cmake.
list(APPEND CMAKE_MODULE_PATH "${EXPAT_lib_DIR}/cmake/modules")


#-----------------------------------------------------------------------
# LibCMaker_<LibName> specific vars and options
#-----------------------------------------------------------------------

#set(SKIP_INSTALL_ALL ON)


#-----------------------------------------------------------------------
# Library specific vars and options
#-----------------------------------------------------------------------

option(BUILD_tools "build the xmlwf tool for expat library" OFF)
option(BUILD_examples "build the examples for expat library" OFF)
option(BUILD_tests "build the tests for expat library" OFF)
# Option BUILD_shared is set in lib_cmaker_expat() by BUILD_SHARED_LIBS.
#option(BUILD_shared "build a shared expat library" ${BUILD_SHARED_LIBS})
option(BUILD_doc "build man page for xmlwf" OFF)
option(USE_libbsd "utilize libbsd (for arc4random_buf)" OFF)
# Option INSTALL is set in lib_cmaker_expat() by NOT SKIP_INSTALL_ALL.
# Set in 'cmr_build_rules_expat' as NOT SKIP_INSTALL_ALL.
#option(INSTALL "install expat files in cmake install target" ${NOT SKIP_INSTALL_ALL})

# Configuration options.
set(XML_CONTEXT_BYTES 1024 CACHE STRING
  "Define to specify how much context to retain around the current parse point")
option(XML_DTD
  "Define to make parameter entity parsing functionality available" ON)
option(XML_NS "Define to make XML Namespaces functionality available" ON)
if(NOT WIN32)
  option(XML_DEV_URANDOM
    "Define to include code reading entropy from `/dev/urandom'." ON)
endif()


#-----------------------------------------------------------------------
# Build, install and find the library
#-----------------------------------------------------------------------

cmr_find_package(
  LibCMaker_DIR   ${LibCMaker_DIR}
  NAME            ${EXPAT_lib_NAME}
  VERSION         ${EXPAT_lib_VERSION}
  LIB_DIR         ${EXPAT_lib_DIR}
  REQUIRED
  FIND_MODULE_NAME EXPAT
)
