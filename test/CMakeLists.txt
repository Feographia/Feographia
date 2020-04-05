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

# Testing with Google Test.
if(BUILD_TESTING)
  set(fgkardia_TARGET "feographia_kardia")

  find_package(GTest REQUIRED)

  set(test_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
  set(font_SRC_DIR "${PROJECT_SOURCE_DIR}/fonts")
  set(data_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/data")
  set(graphics_SRC_DIR "${data_SRC_DIR}")
  if(MINGW)
    set(graphics_SRC_DIR "${graphics_SRC_DIR}/MinGW")
  endif()

  set(test_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  if(MSVC)
    set(test_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
  endif()
  set(font_BIN_DIR "${test_BIN_DIR}/fonts")
  set(data_BIN_DIR "${test_BIN_DIR}/data")

  #add_custom_target(clean_test_temp_dir
  #  COMMAND
  #    ${CMAKE_COMMAND} -E remove_directory "${test_BIN_DIR}/temp"
  #)

  add_custom_target(prepare_test_fonts
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${font_BIN_DIR}"

    # Arimo font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Arimo/Arimo-BoldItalic.ttf"
      "${font_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Arimo/Arimo-Bold.ttf"
      "${font_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Arimo/Arimo-Italic.ttf"
      "${font_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Arimo/Arimo-Regular.ttf"
      "${font_BIN_DIR}"

    # Tinos font files
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Tinos/Tinos-BoldItalic.ttf"
      "${font_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Tinos/Tinos-Bold.ttf"
      "${font_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Tinos/Tinos-Italic.ttf"
      "${font_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${font_SRC_DIR}/Tinos/Tinos-Regular.ttf"
      "${font_BIN_DIR}"
  )

  add_custom_target(prepare_test_font_conf
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_INSTALL_PREFIX}/etc/fonts"
      "${font_BIN_DIR}"
  )

  add_custom_target(prepare_test_data
    COMMAND
      ${CMAKE_COMMAND} -E make_directory "${data_BIN_DIR}"

    # Graphic data
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${graphics_SRC_DIR}/FontTest_1.ppm"
      "${data_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${graphics_SRC_DIR}/FontTest_2.ppm"
      "${data_BIN_DIR}"
  )

  function(add_fg_test test_TARGET test_SRC_FILES test_LIBS)
    add_executable(${test_TARGET} "")
    target_sources(${test_TARGET}
      PRIVATE
        ${test_SRC_FILES}
    )
    set_target_properties(${test_TARGET} PROPERTIES
      CXX_STANDARD 17
    )

    # Test libraries
    target_link_libraries(${test_TARGET} PRIVATE
      ${test_LIBS}
    )

    # HtmlGrapheas
    #target_link_libraries(${test_TARGET} PRIVATE hgraph)

    # Cairo
    target_link_libraries(${test_TARGET} PRIVATE Cairo::Cairo)

    # Pixman
    target_link_libraries(${test_TARGET} PRIVATE Pixman::Pixman)

    # FontConfig
    target_link_libraries(${test_TARGET} PRIVATE Fontconfig::Fontconfig)

    # FreeType
    target_link_libraries(${test_TARGET} PRIVATE Freetype::Freetype)

    # HarfBuzz
    target_include_directories(${test_TARGET} PRIVATE ${HARFBUZZ_INCLUDE_DIR})
    target_link_libraries(${test_TARGET} PRIVATE
      ${HARFBUZZ_LIBRARY} Freetype::Freetype
    )

    if(USE_BOOST)
      target_compile_definitions(${test_TARGET} PRIVATE USE_BOOST)

      # Boost
      target_link_libraries(${test_TARGET} PRIVATE
        Boost::disable_autolinking Boost::filesystem Boost::system
      )
    endif()

    # SQLiteModernCPP
    target_include_directories(
      ${test_TARGET} PRIVATE ${SQLITEMODERNCPP_INCLUDE_DIR}
    )

    # SQLite3
    target_link_libraries(${test_TARGET} PRIVATE sqlite3)

    # ICU
    if(NOT BUILD_SHARED_LIBS)
      target_compile_definitions(${test_TARGET} PRIVATE U_STATIC_IMPLEMENTATION)
    endif()
    target_link_libraries(${test_TARGET} PRIVATE ICU::i18n ICU::uc ICU::data)

    # Expat
    target_include_directories(${test_TARGET} PRIVATE ${EXPAT_INCLUDE_DIR})
    target_link_libraries(${test_TARGET} PRIVATE ${EXPAT_LIBRARY})

    # STLCache
    target_include_directories(${test_TARGET} PRIVATE ${STLCACHE_INCLUDE_DIR})

    # spdlog
    target_link_libraries(${test_TARGET} PRIVATE spdlog::spdlog)

    # fmt
    target_link_libraries(${test_TARGET} PRIVATE fmt::fmt)

    # libpng
    target_link_libraries(${test_TARGET} PRIVATE PNG::PNG)

    # zlib
    target_link_libraries(${test_TARGET} PRIVATE ZLIB::ZLIB)  # icluded in PNG::PNG

    # Threads, pthread
    if(CMAKE_USE_PTHREADS_INIT)
      target_link_libraries(${test_TARGET} PRIVATE Threads::Threads)
    endif()

    # Google Test
    target_link_libraries(${test_TARGET} PRIVATE
      GTest::GTest GTest::Main
    )

    # android_support
    if(ANDROID AND ANDROID_NATIVE_API_LEVEL LESS 21)
      target_link_libraries(${test_TARGET} PRIVATE android_support)
    endif()

    add_dependencies(${test_TARGET}
      #clean_test_temp_dir
      prepare_test_fonts
      prepare_test_font_conf
      prepare_test_data
    )

    add_test(NAME ${test_TARGET} COMMAND ${test_TARGET})
  endfunction()

  # FontLibrary tests
  add_fg_test("FontLibrary_test"
    ${test_SRC_DIR}/Fg/Renderer/FontLibrary/FontLibrary_test.cpp
    ${fgkardia_TARGET}
  )

  # Font tests
  add_fg_test("Font_test"
    ${test_SRC_DIR}/Fg/Renderer/FontLibrary/Font_test.cpp
    ${fgkardia_TARGET}
  )

  # StringUtil tests
  add_fg_test("StringUtil_test"
    ${test_SRC_DIR}/Fg/Util/StringUtil_test.cpp
    ${fgkardia_TARGET}
  )
endif()


# For running test, 1st build all:
#   cmake .. -DBUILD_TESTING=ON
#   cmake --build .
# Then run test with this command:
#   cmake -E env CTEST_OUTPUT_ON_FAILURE=1 cmake --build . --target test
# Or with this command:
#   ctest --verbose
# Env var CTEST_OUTPUT_ON_FAILURE or key --verbose are for show test outputs,
# see
# https://stackoverflow.com/a/37123943
# https://stackoverflow.com/a/38386596

# For Visual Studio:
# https://stackoverflow.com/a/31124523
# https://stackoverflow.com/a/37123943
# cmake -E env CTEST_OUTPUT_ON_FAILURE=1 cmake --build . --target RUN_TESTS
