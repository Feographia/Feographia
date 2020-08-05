#!/bin/bash

set -ev

echo "==== Debug: Begin test_project.sh"

# ==== Run Android emulator ====
#
# http://mywiki.wooledge.org/BashSheet
# [command] & [command]
# Only the command before the & is executed asynchronously
# and you must not put a ';' after the '&', the '&' replaces the ';'.
if [[ ${cmr_TARGET_OS} == "Android" ]] ; then
  echo "==== Debug: Start Android emulator"

  # NOTE: 'arm64-v8a' emulator (any API level) does not start on Linux
  #       with success, boot animation is not ending.
  # NOTE: 'armeabi-v7a' API 16 emulator on Linux executes programs regularly
  #       with "Illegal instruction". API 24 on Linux works fine.
  # NOTE: 'x86' API 24 emulator does not start on Linux with success.
  #       API 23 on Linux works fine.
  echo | avdmanager create avd -f -n fg_test -c 1024M -k "system-images;android-${cmr_ANDROID_EMULATOR_API_LEVEL};default;${cmr_ANDROID_ABI}"
  emulator -avd fg_test -memory 1024 -no-window -gpu auto -no-accel -no-snapshot -no-audio -camera-back none -camera-front none &
  ${cmr_REPO_DIR}/libs/LibCMaker/ci/android-wait-for-emulator.sh
  adb shell input keyevent 82 &
fi

#if [[ ${cmr_ANDROID_ABI} == "arm64-v8a" ]]; then travis_terminate 0 ; fi

# ==== Run iOS emulator ====
#if [[ ${cmr_TARGET_OS} == "iOS" ]]; then
#  # TODO: run iOS emulator.
#fi


# ==== Run tests ====
cd ${cmr_BUILD_DIR}

if [[  ( ${cmr_TARGET_OS} == "Linux" )
    || ( ${cmr_TARGET_OS} == "Windows" && ${cmr_MINGW} == "ON" )
    || ( ${cmr_TARGET_OS} == "macOS" && ${cmr_UNIX_MAKE_FILES} == "ON" )
    || ( ${cmr_TARGET_OS} == "Android" )
    ]] ; then
  echo "==== Debug: Run tests for Unix-like generators"

  ${cmr_CTEST_CMD} --output-on-failure
fi

if [[  ( ${cmr_TARGET_OS} == "Windows" && ${cmr_MSVC} == "ON" )
    || ( ${cmr_TARGET_OS} == "macOS" && ${cmr_XCODE} == "ON" )
    || ( ${cmr_TARGET_OS} == "iOS" )
    ]] ; then
  echo "==== Debug: Run tests for MSVC or Xcode generators"

  ${cmr_CTEST_CMD} --output-on-failure --build-config ${cmr_CMAKE_BUILD_TYPE}
fi

echo "==== Debug: End test_project.sh"
