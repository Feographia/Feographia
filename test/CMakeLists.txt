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

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${graphics_SRC_DIR}/LiteHtmlContainer_1.ppm"
      "${data_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${graphics_SRC_DIR}/LiteHtmlContainer_2.ppm"
      "${data_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${graphics_SRC_DIR}/LiteHtmlDocument_1.ppm"
      "${data_BIN_DIR}"

    # master.css
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_CURRENT_LIST_DIR}/data/master.css"
      "${data_BIN_DIR}"
  )

  function(add_fg_test_executable
      test_TARGET test_SRC_FILES test_LIBS out_TEST_LIST)
    add_executable(${test_TARGET} "")
    target_sources(${test_TARGET}
      PRIVATE
        ${test_SRC_FILES}
    )
    set_target_properties(${test_TARGET} PROPERTIES
      CXX_STANDARD 17
    )
    if(NOT MINGW AND NOT ANDROID AND NOT IOS
        AND NOT (APPLE AND CMAKE_GENERATOR MATCHES "Unix Makefiles"))
      set_target_properties(${test_TARGET} PROPERTIES
        # Link all libraries into the target so as not to use LD_LIBRARY_PATH.
        LINK_WHAT_YOU_USE ON
      )
    endif()

    # Test libraries
    target_link_libraries(${test_TARGET} PRIVATE
      ${test_LIBS}
    )

    # HtmlGrapheas
    #target_link_libraries(${test_TARGET} PRIVATE hgraph)

    # litehtml, gumbo (embedded into litehtml)
    target_link_libraries(${test_TARGET} PRIVATE litehtml::litehtml)

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

    list(APPEND ${out_TEST_LIST} ${test_TARGET})
    set(${out_TEST_LIST} ${${out_TEST_LIST}} PARENT_SCOPE)
  endfunction()

  set(fg_TEST_LIST)

  # Util tests
  add_fg_test_executable("StringUtil_test"
    ${test_SRC_DIR}/Fg/Util/StringUtil_test.cpp
    ${fgkardia_TARGET}
    fg_TEST_LIST
  )

  # FontLibrary tests
  add_fg_test_executable("FontLibrary_test"
    ${test_SRC_DIR}/Fg/Renderer/FontLibrary/FontLibrary_test.cpp
    ${fgkardia_TARGET}
    fg_TEST_LIST
  )
  add_fg_test_executable("Font_test"
    ${test_SRC_DIR}/Fg/Renderer/FontLibrary/Font_test.cpp
    ${fgkardia_TARGET}
    fg_TEST_LIST
  )

  # litehtml renderer tests
  add_fg_test_executable("LiteHtmlContainer_test"
    ${test_SRC_DIR}/Fg/Renderer/litehtml/LiteHtmlContainer_test.cpp
    ${fgkardia_TARGET}
    fg_TEST_LIST
  )
  add_fg_test_executable("LiteHtmlRenderer_test"
    ${test_SRC_DIR}/Fg/Renderer/litehtml/LiteHtmlRenderer_test.cpp
    ${fgkardia_TARGET}
    fg_TEST_LIST
  )

  if(wxWidgets_PLATFORM)
    foreach(fg_test IN LISTS fg_TEST_LIST)
      add_test(NAME ${fg_test} COMMAND ${fg_test})
    endforeach()
  endif()

  if(ANDROID)
    find_program(adb_exec adb)
    if(NOT adb_exec)
      message(FATAL_ERROR "Could not find 'adb'")
    endif()

    set(TEST_WORK_DIR "/data/local/tmp/fg_test")

    add_test(NAME rm_work_dir
      COMMAND ${adb_exec} shell "if [ -d \"${TEST_WORK_DIR}\" ] ; then rm -r \"${TEST_WORK_DIR}\" ; fi"
    )

    add_test(NAME push_fonts
      COMMAND ${adb_exec} push "${font_BIN_DIR}" "${TEST_WORK_DIR}/fonts"
    )
    add_test(NAME push_data
      COMMAND ${adb_exec} push "${data_BIN_DIR}" "${TEST_WORK_DIR}/data"
    )
    add_test(NAME push_icu_data
      COMMAND ${adb_exec} push
        "${cmr_INSTALL_DIR}/share/icu"
        "${TEST_WORK_DIR}/share/icu"
    )
    if(BUILD_SHARED_LIBS)
      # Fix for the adb error on the arm devices if use the adb push
      # for the directory with the soft symlinks:
      # adb: error: failed to copy '<soft symlink>' to '/data/local/tmp/<soft symlink>': remote symlink failed: Permission denied
      # Instead of the direct using of the adb push use tar.
      execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar cf
          "${CMAKE_CURRENT_BINARY_DIR}/lib.tar"
          "${cmr_INSTALL_DIR}/lib"
        WORKING_DIRECTORY "${cmr_INSTALL_DIR}"
      )
      add_test(NAME push_libs_tar
        COMMAND ${adb_exec} push
          "${CMAKE_CURRENT_BINARY_DIR}/lib.tar"
          "${TEST_WORK_DIR}/lib.tar"
      )
      add_test(NAME extract_libs_tar
        COMMAND ${adb_exec} shell tar --no-same-owner
          -xf "${TEST_WORK_DIR}/lib.tar"
          -C "${TEST_WORK_DIR}"
      )

      add_test(NAME push_libfeographia_kardia
        COMMAND ${adb_exec} push
          "${PROJECT_BINARY_DIR}/Feographia/libfeographia_kardia.so"
          "${TEST_WORK_DIR}/lib/libfeographia_kardia.so"
      )

      find_library(cpp_shared_LIB "c++_shared")
      if(NOT cpp_shared_LIB)
        message(FATAL_ERROR "The library 'c++_shared' can not be found.")
      endif()

      get_filename_component(cpp_shared_LIB_FILE_NAME ${cpp_shared_LIB} NAME)

      add_test(NAME push_cpp_shared
        COMMAND ${adb_exec} push
          "${cpp_shared_LIB}"
          "${TEST_WORK_DIR}/lib/${cpp_shared_LIB_FILE_NAME}"
      )
    endif()

    # 1st -- push all executable files.
    foreach(fg_test IN LISTS fg_TEST_LIST)
      add_test(NAME push_${fg_test}
        COMMAND ${adb_exec} push ${fg_test} "${TEST_WORK_DIR}/${fg_test}"
      )
      add_test(NAME chmod_${fg_test}
        COMMAND ${adb_exec} shell chmod 775 "${TEST_WORK_DIR}/${fg_test}"
      )
    endforeach()

    # 2nd -- run tests.
    foreach(fg_test IN LISTS fg_TEST_LIST)
      add_test(NAME ${fg_test} COMMAND ${adb_exec} shell
        "cd ${TEST_WORK_DIR} && "
        "LD_LIBRARY_PATH=${TEST_WORK_DIR}/lib ${TEST_WORK_DIR}/${fg_test}"
        # :${LD_LIBRARY_PATH} || :/vendor/lib64:/system/lib64
      )
      set_tests_properties(${fg_test} PROPERTIES
        PASS_REGULAR_EXPRESSION "PASSED"
      )
      set_tests_properties(${fg_test} PROPERTIES
        FAIL_REGULAR_EXPRESSION "FAILED"
      )
    endforeach()
  endif()
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
