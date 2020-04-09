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

set(fgkardia_TARGET "feographia_kardia")

include(GNUInstallDirs)


#-----------------------------------------------------------------------
# Set vars to Feographia
#-----------------------------------------------------------------------

if(NOT FT_WITH_HARFBUZZ)
  message(FATAL_ERROR
    "Please set FT_WITH_HARFBUZZ=ON and build FreeType with HarfBuzz."
  )
endif()


#-----------------------------------------------------------------------
# Feographia
#-----------------------------------------------------------------------

set(private_src_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(interface_src_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(build_src_DIR
  "$<BUILD_INTERFACE:${private_src_DIR}>"
)
set(install_src_DIR
  "$<INSTALL_INTERFACE:${interface_src_DIR}>"
)
set(public_src_DIR "${build_src_DIR}${install_src_DIR}")

add_library(${fgkardia_TARGET} "")
set_target_properties(${fgkardia_TARGET} PROPERTIES
  CXX_STANDARD 17
)
# Export Feographia's include dirs.
target_include_directories(${fgkardia_TARGET} PUBLIC ${public_src_DIR})

target_sources(${fgkardia_TARGET}
  PRIVATE
#    ${private_src_DIR}/Fg/Module/Bible/Base/Deque.cpp
#    ${private_src_DIR}/Fg/Module/Bible/Base/Fragment.cpp
    ${private_src_DIR}/Fg/Module/Bible/Base/View.cpp

#    ${private_src_DIR}/Fg/Module/Bible/Block/Block.cpp
#    ${private_src_DIR}/Fg/Module/Bible/Block/BlockDeque.cpp
#    ${private_src_DIR}/Fg/Module/Bible/Block/BlockView.cpp

    ${private_src_DIR}/Fg/Module/Bible/Block/Word/Item.cpp
#    ${private_src_DIR}/Fg/Module/Bible/Block/Word/ItemDeque.cpp
    ${private_src_DIR}/Fg/Module/Bible/Block/Word/ItemView.cpp
    ${private_src_DIR}/Fg/Module/Bible/Block/Word/WordPartView.cpp

    ${private_src_DIR}/Fg/Renderer/Cairo.cpp

    ${private_src_DIR}/Fg/Renderer/FontLibrary/Font.cpp
    ${private_src_DIR}/Fg/Renderer/FontLibrary/FontLibrary.cpp

    ${private_src_DIR}/Fg/Util/FileUtil.cpp

    ${private_src_DIR}/Fg/Renderer/litehtml/LiteHtmlContainer.cpp
    ${private_src_DIR}/Fg/Renderer/litehtml/LiteHtmlRenderer.cpp

  PUBLIC
    ${public_src_DIR}/Fg/Shared/Types.h

#    ${public_src_DIR}/Fg/Module/Bible/Base/Deque.h
#    ${public_src_DIR}/Fg/Module/Bible/Base/Fragment.h
    ${public_src_DIR}/Fg/Module/Bible/Base/View.h

#    ${public_src_DIR}/Fg/Module/Bible/Block/Block.h
#    ${public_src_DIR}/Fg/Module/Bible/Block/BlockDeque.h
#    ${public_src_DIR}/Fg/Module/Bible/Block/BlockView.h

    ${public_src_DIR}/Fg/Module/Bible/Block/Word/Item.h
#    ${public_src_DIR}/Fg/Module/Bible/Block/Word/ItemDeque.h
    ${public_src_DIR}/Fg/Module/Bible/Block/Word/ItemView.h
    ${public_src_DIR}/Fg/Module/Bible/Block/Word/WordPartView.h

    ${public_src_DIR}/Fg/Renderer/Cairo.h

    ${public_src_DIR}/Fg/Renderer/FontLibrary/Font.h
    ${public_src_DIR}/Fg/Renderer/FontLibrary/FontLibrary.h

    ${public_src_DIR}/Fg/Util/FileUtil.h
    ${public_src_DIR}/Fg/Util/Filesystem.h
    ${public_src_DIR}/Fg/Util/StringUtil.h

    ${public_src_DIR}/Fg/Renderer/litehtml/LiteHtmlContainer.h
    ${public_src_DIR}/Fg/Renderer/litehtml/LiteHtmlRenderer.h
)


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

# litehtml
target_link_libraries(${fgkardia_TARGET} PRIVATE litehtml::litehtml)

# Cairo
target_link_libraries(${fgkardia_TARGET} PRIVATE Cairo::Cairo)

# Pixman
target_link_libraries(${fgkardia_TARGET} PRIVATE Pixman::Pixman)

# FontConfig
target_link_libraries(${fgkardia_TARGET} PRIVATE Fontconfig::Fontconfig)

# FreeType
target_link_libraries(${fgkardia_TARGET} PRIVATE Freetype::Freetype)

# HarfBuzz
target_include_directories(${fgkardia_TARGET} PRIVATE ${HARFBUZZ_INCLUDE_DIR})
# From <freetype sources>/docs/CHANGES:
#  Note that HarfBuzz depends on FreeType.
target_link_libraries(${fgkardia_TARGET} PRIVATE
  ${HARFBUZZ_LIBRARY} Freetype::Freetype
)

if(USE_BOOST)
  target_compile_definitions(${fgkardia_TARGET} PRIVATE USE_BOOST)

  # Boost
  target_link_libraries(${fgkardia_TARGET} PRIVATE
    Boost::disable_autolinking Boost::filesystem Boost::system
  )
endif()

# SQLiteModernCPP
target_include_directories(
  ${fgkardia_TARGET} PRIVATE ${SQLITEMODERNCPP_INCLUDE_DIR}
)

# SQLite3
target_link_libraries(${fgkardia_TARGET} PRIVATE sqlite3)

# ICU
if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(${fgkardia_TARGET} PRIVATE U_STATIC_IMPLEMENTATION)
endif()
target_link_libraries(${fgkardia_TARGET} PRIVATE ICU::i18n ICU::uc ICU::data)

# Expat
target_include_directories(${fgkardia_TARGET} PRIVATE ${EXPAT_INCLUDE_DIR})
target_link_libraries(${fgkardia_TARGET} PRIVATE ${EXPAT_LIBRARY})

# STLCache
target_include_directories(${fgkardia_TARGET} PRIVATE ${STLCACHE_INCLUDE_DIR})

# spdlog
target_link_libraries(${fgkardia_TARGET} PRIVATE spdlog::spdlog)

# fmt
target_link_libraries(${fgkardia_TARGET} PRIVATE fmt::fmt)

# libpng
target_link_libraries(${fgkardia_TARGET} PRIVATE PNG::PNG)

# zlib
target_link_libraries(${fgkardia_TARGET} PRIVATE ZLIB::ZLIB)  # icluded in PNG::PNG

if(MSVC)
  # Dirent
  target_include_directories(${fgkardia_TARGET} PRIVATE ${DIRENT_INCLUDE_DIR})
endif()

# Threads, pthread
if(CMAKE_USE_PTHREADS_INIT)
  target_link_libraries(${fgkardia_TARGET} PRIVATE Threads::Threads)
endif()

# android_support
if(ANDROID AND ANDROID_NATIVE_API_LEVEL LESS 21)
  target_link_libraries(${fgkardia_TARGET} PRIVATE android_support)
endif()


#-----------------------------------------------------------------------
# Platforms
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# wxWidgets
#-----------------------------------------------------------------------

# To run the application on Linux with the shared libs which are installed
# to the non standard path specified in CMAKE_INSTALL_PREFIX use:
# LD_LIBRARY_PATH="<${CMAKE_INSTALL_PREFIX}>/lib:.:$LD_LIBRARY_PATH" ./html_grapheas
# where <${CMAKE_INSTALL_PREFIX}> is path which is used
# as value for CMAKE_INSTALL_PREFIX.

if(wxWidgets_PLATFORM)
  # FeographiaAppWx
  set(FgAppWx_DIR ${CMAKE_CURRENT_LIST_DIR}/Fg/platform/wxwidgets)
#  add_subdirectory("${FgAppWx_DIR}" "${PROJECT_BINARY_DIR}/FeographiaAppWx")
endif()
