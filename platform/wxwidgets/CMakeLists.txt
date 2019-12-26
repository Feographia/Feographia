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

cmake_minimum_required(VERSION 3.8)
# C++17 compiler:
#   GCC 8
#   Clang 8 (Android NDK r19)
#   Apple Clang 11.0.0, Xcode 11.0
#   MSVC 19.15, VS 2017 15.8
# See: https://en.cppreference.com/w/cpp/compiler_support

project(FeographiaAppWx CXX)

set(app_NAME "feographia")

if(NOT HGKamva_DIR)
  message(FATAL_ERROR
    "Please set HGKamva_DIR with path to HtmlGrapheasKamva library.")
endif()


#-----------------------------------------------------------------------
# Set vars to FeographiaAppWx
#-----------------------------------------------------------------------


#-----------------------------------------------------------------------
# FeographiaAppWx
#-----------------------------------------------------------------------

set(private_src_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(interface_src_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(build_src_DIR
  "$<BUILD_INTERFACE:${private_src_DIR}>"
)
set(install_src_DIR
  "$<INSTALL_INTERFACE:${interface_src_DIR}>"
)
set(public_src_DIR "${build_src_DIR}${install_src_DIR}")

if(WIN32 AND NOT ATTACH_WX_CONSOLE)
  # See docs for add_executable().
  set(WIN32GUI WIN32)
else()
  set(WIN32GUI "")
endif()

add_executable(${app_NAME} ${WIN32GUI} "")
set_target_properties(${app_NAME} PROPERTIES
  CXX_STANDARD 17
)
set_property(TARGET ${app_NAME}
  PROPERTY RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
)
if(ATTACH_WX_CONSOLE)
  target_compile_definitions(${app_NAME} PRIVATE ATTACH_WX_CONSOLE)
endif()

target_include_directories(${app_NAME} PRIVATE ${private_src_DIR})

target_sources(${app_NAME}
  PRIVATE
    ${private_src_DIR}/fgapp/main.cpp
)


#-----------------------------------------------------------------------
# Link to the libraries
#-----------------------------------------------------------------------

# HtmlGrapheasKamvaWx
target_link_libraries(${app_NAME} PRIVATE hgkamva_wx)

# wxWidgets
if(WX_USE_FIND_PACKAGE_MODULE)
  find_package(wxWidgets REQUIRED)

  #include(${wxWidgets_USE_FILE})
  #target_link_libraries(${app_NAME} ${wxWidgets_LIBRARIES})

  target_compile_definitions(${app_NAME} PRIVATE ${wxWidgets_DEFINITIONS})
  target_compile_definitions(${app_NAME} PRIVATE
      $<$<CONFIG:Debug>:${wxWidgets_DEFINITIONS_DEBUG}>
  )
  target_compile_options(${app_NAME} PRIVATE ${wxWidgets_CXX_FLAGS})
  target_include_directories(${app_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
  target_link_libraries(${app_NAME} PRIVATE ${wxWidgets_LIBRARIES})

  if(MINGW AND NOT BUILD_SHARED_LIBS)
    if(NOT MINGW_HOME)
      message(FATAL_ERROR "Please set MINGW_HOME to MinGW home path.")
    endif()

    set(mingw_ARCH "x86_64")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
      set(mingw_ARCH "i686")
    endif()
    set(mingw_PREFIX "${mingw_ARCH}-w64-mingw32")
    list(APPEND CMAKE_PREFIX_PATH "${MINGW_HOME}/${mingw_PREFIX}")

    find_library(comdlg32_LIB comdlg32)
    if(comdlg32_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${comdlg32_LIB})
    endif()

    find_library(gdi32_LIB gdi32)
    if(gdi32_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${gdi32_LIB})
    endif()

    find_library(ole32_LIB ole32)
    if(ole32_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${ole32_LIB})
    endif()

    find_library(oleacc_LIB oleacc)
    if(oleacc_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${oleacc_LIB})
    endif()

    find_library(oleaut32_LIB oleaut32)
    if(oleaut32_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${oleaut32_LIB})
    endif()

    find_library(uuid_LIB uuid)
    if(uuid_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${uuid_LIB})
    endif()

    find_library(uxtheme_LIB uxtheme)
    if(uxtheme_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${uxtheme_LIB})
    endif()

    find_library(winspool_LIB winspool)
    if(winspool_LIB)
      target_link_libraries(${app_NAME} PRIVATE ${winspool_LIB})
    endif()
  endif()
else()
  target_link_libraries(${app_NAME} PRIVATE ${WX_lib_COMPONENTS})
endif()


#-----------------------------------------------------------------------
# Test data to debug of app running
#-----------------------------------------------------------------------

if(BUILD_TESTING)
#[==[  # TODO: Use litehtml for viewing of dictionaries and other HTML texts.
  set(WX_FG_TEST_DIR "${PROJECT_BINARY_DIR}")
  if(MSVC)
    set(WX_FG_TEST_DIR "${PROJECT_BINARY_DIR}/$<CONFIG>")
  endif()
  set(WX_FG_TEST_FONT_DIR "${WX_FG_TEST_DIR}/fonts")
  set(WX_FG_TEST_DATA_DIR "${WX_FG_TEST_DIR}/data")

  add_custom_target(prepare_wx_test_fonts
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${WX_FG_TEST_FONT_DIR}"

    # Arimo font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-BoldItalic.ttf"
      "${WX_FG_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-Bold.ttf"
      "${WX_FG_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-Italic.ttf"
      "${WX_FG_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Arimo/Arimo-Regular.ttf"
      "${WX_FG_TEST_FONT_DIR}"

    # Tinos font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-BoldItalic.ttf"
      "${WX_FG_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-Bold.ttf"
      "${WX_FG_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-Italic.ttf"
      "${WX_FG_TEST_FONT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/fonts/Tinos/Tinos-Regular.ttf"
      "${WX_FG_TEST_FONT_DIR}"
  )

  add_custom_target(prepare_wx_test_font_conf
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_INSTALL_PREFIX}/etc/fonts"
      "${WX_FG_TEST_FONT_DIR}"
  )

  add_custom_target(prepare_wx_test_data
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${WX_FG_TEST_DATA_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/test/data/master.css"
      "${WX_FG_TEST_DATA_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${HGKamva_DIR}/test/data/test.html"
      "${WX_FG_TEST_DATA_DIR}"
  )

  add_dependencies(${app_NAME}
    prepare_wx_test_fonts
    prepare_wx_test_font_conf
    prepare_wx_test_data
  )
]==]
endif()
