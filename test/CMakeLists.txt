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


# For the test running, 1st build all:
#   cmake .. -DBUILD_TESTING=ON
#   cmake --build .
# Then run the test with this command:
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


# Testing with Google Test.
if(BUILD_TESTING)
  set(fgkardia_TARGET "feographia_kardia")
  set(test_TARGET "FgUnitTests")

  find_package(GTest REQUIRED)


  #-----------------------------------------------------------------------
  # Test data
  #-----------------------------------------------------------------------

  set(test_SRC_DIR "${PROJECT_SOURCE_DIR}/src")
  set(font_SRC_DIR "${PROJECT_SOURCE_DIR}/fonts")
  set(data_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/data")
  set(graphics_SRC_DIR "${data_SRC_DIR}/graphics_1")
  if(MINGW OR (ANDROID AND
      (ANDROID_ABI STREQUAL "armeabi-v7a" OR ANDROID_ABI STREQUAL "x86")))
    # graphics_1 and graphics_2 differ in some bits.
    set(graphics_SRC_DIR "${data_SRC_DIR}/graphics_2")
  endif()

  set(test_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  if(MSVC OR XCODE)
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
      "${cmr_INSTALL_DIR}/etc/fonts"
      # TODO: to "${test_BIN_DIR}/etc/fonts"
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


  #-----------------------------------------------------------------------
  # Test executable
  #-----------------------------------------------------------------------

  if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
       OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
      -Wno-global-constructors
    )
  endif()

  add_executable(${test_TARGET} ${IOS_MACOSX_BUNDLE} "")

  target_sources(${test_TARGET}
    PRIVATE
      ${test_SRC_DIR}/Fg/UnitTests/main.cpp
      ${test_SRC_DIR}/Fg/Util/StringUtil_test.cpp
      ${test_SRC_DIR}/Fg/Module/Bible/Text/Text_test.cpp
      ${test_SRC_DIR}/Fg/Renderer/FontLibrary/FontLibrary_test.cpp
      ${test_SRC_DIR}/Fg/Renderer/FontLibrary/Font_test.cpp
      ${test_SRC_DIR}/Fg/Renderer/litehtml/LiteHtmlContainer_test.cpp
      ${test_SRC_DIR}/Fg/Renderer/litehtml/LiteHtmlRenderer_test.cpp
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

  if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
       OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set_source_files_properties(
      ${test_SRC_DIR}/Fg/UnitTests/main.cpp
      PROPERTIES COMPILE_OPTIONS
      "-Wno-exit-time-destructors"
    )
  endif()

  if(MSVC)
    target_compile_options(${test_TARGET} PRIVATE
      -wd4251 -wd4275
      # TODO: how to move -wd4388 and -wd4668 from main options to unit tests?
    )
  endif()

  # feographia_kardia
  target_link_libraries(${test_TARGET} PRIVATE
    ${fgkardia_TARGET}
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


  #-----------------------------------------------------------------------
  # Run tests for Linux, Windows, macOS
  #-----------------------------------------------------------------------

  if(wxWidgets_PLATFORM)
    add_test(NAME ${test_TARGET} COMMAND ${test_TARGET})
  endif()


  #-----------------------------------------------------------------------
  # Run tests for Android
  #-----------------------------------------------------------------------

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
      add_test(NAME check_tar
        COMMAND ${adb_exec} shell tar --help
      )
      set_tests_properties(check_tar PROPERTIES
        PASS_REGULAR_EXPRESSION "usage: tar"
      )
      set_tests_properties(check_tar PROPERTIES
        FAIL_REGULAR_EXPRESSION "tar: not found"
      )

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

    add_test(NAME push_${test_TARGET}
      COMMAND ${adb_exec} push ${test_TARGET} "${TEST_WORK_DIR}/${test_TARGET}"
    )
    add_test(NAME chmod_${test_TARGET}
      COMMAND ${adb_exec} shell chmod 775 "${TEST_WORK_DIR}/${test_TARGET}"
    )
    add_test(NAME ${test_TARGET} COMMAND ${adb_exec} shell
      "cd ${TEST_WORK_DIR} && "
      "LD_LIBRARY_PATH=${TEST_WORK_DIR}/lib ${TEST_WORK_DIR}/${test_TARGET}"
      # :${LD_LIBRARY_PATH} || :/vendor/lib64:/system/lib64
    )

#    add_test(NAME pull_test_result_11
#      COMMAND ${adb_exec} pull
#        "${TEST_WORK_DIR}/FontTest_1.ppm"
#        "${CMAKE_CURRENT_BINARY_DIR}/FontTest_1.ppm"
#    )
#    add_test(NAME pull_test_result_12
#      COMMAND ${adb_exec} pull
#        "${TEST_WORK_DIR}/FontTest_2.ppm"
#        "${CMAKE_CURRENT_BINARY_DIR}/FontTest_2.ppm"
#    )
#    add_test(NAME pull_test_result_21
#      COMMAND ${adb_exec} pull
#        "${TEST_WORK_DIR}/LiteHtmlContainer_1.ppm"
#        "${CMAKE_CURRENT_BINARY_DIR}/LiteHtmlContainer_1.ppm"
#    )
#    add_test(NAME pull_test_result_22
#      COMMAND ${adb_exec} pull
#        "${TEST_WORK_DIR}/LiteHtmlContainer_2.ppm"
#        "${CMAKE_CURRENT_BINARY_DIR}/LiteHtmlContainer_2.ppm"
#    )
#    add_test(NAME pull_test_result_31
#      COMMAND ${adb_exec} pull
#        "${TEST_WORK_DIR}/LiteHtmlDocument_1.ppm"
#        "${CMAKE_CURRENT_BINARY_DIR}/LiteHtmlDocument_1.ppm"
#    )
  endif()


  #-----------------------------------------------------------------------
  # Run tests for iOS
  #-----------------------------------------------------------------------

  if(IOS)
    set(IOS_TEST_BUNDLE_IDENTIFIER "${IOS_APP_BUNDLE_IDENTIFIER}.unit_tests")
    set(IOS_TEST_APP_BIN_DIR
      "${PROJECT_BINARY_DIR}/test/\${CONFIGURATION}\${EFFECTIVE_PLATFORM_NAME}/${test_TARGET}.app"
    )
    set(IOS_TEST_APP_FULL_BIN_DIR
      "${PROJECT_BINARY_DIR}/test/$<CONFIG>-iphonesimulator/${test_TARGET}.app"
    )

    set_target_properties(${test_TARGET} PROPERTIES
      MACOSX_BUNDLE_INFO_STRING "${IOS_TEST_BUNDLE_IDENTIFIER}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${IOS_TEST_BUNDLE_IDENTIFIER}"
      MACOSX_BUNDLE_BUNDLE_NAME "${IOS_TEST_BUNDLE_IDENTIFIER}"

      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${IOS_CODE_SIGN_IDENTITY}"
      XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
      XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET}
      XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${IOS_DEVICE_FAMILY}
    )

    add_custom_command(TARGET ${test_TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
        "${IOS_TEST_APP_BIN_DIR}/fonts"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${font_BIN_DIR}"
        "${IOS_TEST_APP_BIN_DIR}/fonts/"

      COMMAND ${CMAKE_COMMAND} -E make_directory
        "${IOS_TEST_APP_BIN_DIR}/data"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${data_BIN_DIR}"
        "${IOS_TEST_APP_BIN_DIR}/data/"

      COMMAND ${CMAKE_COMMAND} -E make_directory
        "${IOS_TEST_APP_BIN_DIR}/share/icu"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${cmr_INSTALL_DIR}/share/icu"
        "${IOS_TEST_APP_BIN_DIR}/share/icu/"
    )

    if(BUILD_SHARED_LIBS)
      add_custom_command(TARGET ${test_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
          "${IOS_TEST_APP_BIN_DIR}/lib"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${cmr_INSTALL_DIR}/lib"
          "${IOS_TEST_APP_BIN_DIR}/lib/"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${PROJECT_BINARY_DIR}/Feographia/$<CONFIG>-iphonesimulator/libfeographia_kardia.dylib"
          "${IOS_TEST_APP_BIN_DIR}/lib/libfeographia_kardia.dylib"
      )
    endif()

    if(IOS_CODE_SIGN_IDENTITY)
      # Codesign the framework in it's new spot
      add_custom_command(TARGET ${test_TARGET} POST_BUILD
        #COMMAND codesign --force --verbose
        #    ${IOS_TEST_APP_BIN_DIR}/Frameworks/${FRAMEWORK_NAME}.framework
        #    --sign ${IOS_CODE_SIGN_IDENTITY}
        #COMMAND codesign --force --verbose
        #    \${BUILT_PRODUCTS_DIR}/${test_TARGET}.app/Frameworks/${FRAMEWORK_NAME}.framework
        #    --sign ${IOS_CODE_SIGN_IDENTITY}
        COMMAND codesign --force --verbose
            ${IOS_TEST_APP_BIN_DIR}/${test_TARGET}
            --sign ${IOS_CODE_SIGN_IDENTITY}
      )
    endif()

    #if(run with spawn)
    #  add_test(NAME copy_icu_dat_file_of_${test_TARGET}
    #    COMMAND bash -c
    #      "TEST_APP_HOME_DIR=$(xcrun simctl getenv booted HOME) && cp -R ${cmr_INSTALL_DIR}/share $TEST_APP_HOME_DIR"
    #  )
    #  add_test(NAME ${test_TARGET}
    #    COMMAND xcrun simctl spawn booted
    #      ${IOS_TEST_APP_FULL_BIN_DIR}/${test_TARGET}
    #  )
    #
    #else()  # With install app.
      add_test(NAME install_${test_TARGET}
        COMMAND xcrun simctl install booted ${IOS_TEST_APP_FULL_BIN_DIR}/
      )

      # Must be after the app installing.
      #add_test(NAME copy_icu_dat_file_of_${test_TARGET}
      #  COMMAND bash -c
      #    "TEST_APP_HOME_DIR=$(xcrun simctl get_app_container booted ${IOS_TEST_BUNDLE_IDENTIFIER} data) && cp -R ${cmr_INSTALL_DIR}/share $TEST_APP_HOME_DIR"
      #)

      add_test(NAME ${test_TARGET}
        COMMAND xcrun simctl launch --console-pty booted
          ${IOS_TEST_BUNDLE_IDENTIFIER}
      )
    #endif()

    # NOTE: Use '--console-pty' with 'xcrun simctl launch' for Travis CI,
    # not '--console'. With '--console' is error:
    # -----------------------------------------------------------------------
    # Unable to establish FIFO: Error 2
    # An error was encountered processing the command (domain=NSPOSIXErrorDomain, code=2):
    # The operation couldn.t be completed. No such file or directory
    # No such file or directory
    # -----------------------------------------------------------------------
  endif()


  #-----------------------------------------------------------------------
  # Common test settings
  #-----------------------------------------------------------------------

  set_tests_properties(${test_TARGET} PROPERTIES
    PASS_REGULAR_EXPRESSION "PASSED"
  )
  set_tests_properties(${test_TARGET} PROPERTIES
    FAIL_REGULAR_EXPRESSION "FAILED"
  )
endif()
