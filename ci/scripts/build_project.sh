#!/bin/bash

set -ev

echo "==== Debug: Begin build_project.sh"

${cmr_CMAKE_CMD} --version

cd ${cmr_BUILD_DIR}

if [[ ${cmr_TARGET_OS} == "Linux" ]] ; then
  echo "==== Debug: Build Project for Linux"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
      -DCMAKE_INTERPROCEDURAL_OPTIMIZATION:BOOL=${cmr_CMAKE_INTERPROCEDURAL_OPTIMIZATION} \

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT}
  fi
fi

if [[ ${cmr_TARGET_OS} == "Windows" && ${cmr_MSVC} == "ON" ]] ; then
  echo "==== Debug: Build Project for Windows, MSVC"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -Dcmr_VS_GENERATOR_VERBOSITY_LEVEL:STRING="normal" \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \
      -DCMAKE_GENERATOR_PLATFORM:STRING="${cmr_CMAKE_GENERATOR_PLATFORM}" \
      -DCMAKE_GENERATOR_TOOLSET:STRING="${cmr_CMAKE_GENERATOR_TOOLSET}" \
      -DCMAKE_CONFIGURATION_TYPES:STRING="${cmr_CMAKE_BUILD_TYPE}" \
    -DATTACH_WX_CONSOLE:BOOL=${cmr_ATTACH_WX_CONSOLE} \

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT} --config ${cmr_CMAKE_BUILD_TYPE}
  fi
fi

if [[ ${cmr_TARGET_OS} == "Windows" && ${cmr_MINGW} == "ON" ]] ; then
  echo "==== Debug: Build Project for Windows, MinGW-w64"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \
    -DATTACH_WX_CONSOLE:BOOL=${cmr_ATTACH_WX_CONSOLE} \

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT}
  fi
fi

if [[ ${cmr_TARGET_OS} == "macOS" && ${cmr_XCODE} == "ON" ]] ; then
  echo "==== Debug: Build Project for macOS, Xcode"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \
      -DCMAKE_CONFIGURATION_TYPES:STRING="${cmr_CMAKE_BUILD_TYPE}" \
    -Dcmr_XCODE_GENERATOR_VERBOSITY_LEVEL:STRING="-quiet" \

    # The job exceeded the maximum log length, and has been terminated.
    #-Dcmr_XCODE_GENERATOR_VERBOSITY_LEVEL:STRING="-quiet"

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT} --config ${cmr_CMAKE_BUILD_TYPE}
  fi
fi

if [[ ${cmr_TARGET_OS} == "macOS" && ${cmr_UNIX_MAKE_FILES} == "ON" ]] ; then
  echo "==== Debug: Build Project for macOS, Unix Makefiles"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT}
  fi
fi

if [[ ${cmr_TARGET_OS} == "Android" ]] ; then
  echo "==== Debug: Build Project for Android"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -DANDROID_NDK:PATH=${cmr_ANDROID_NDK} \
      -DCMAKE_TOOLCHAIN_FILE:PATH=${cmr_CMAKE_TOOLCHAIN_FILE} \
      -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \
      -DANDROID_ABI:STRING="${cmr_ANDROID_ABI}" \
      -DANDROID_NATIVE_API_LEVEL:STRING=${cmr_ANDROID_NATIVE_API_LEVEL} \
      -DANDROID_TOOLCHAIN:STRING="${cmr_ANDROID_TOOLCHAIN}" \
      -DANDROID_STL:STRING="${cmr_ANDROID_STL}" \
      -DANDROID_CPP_FEATURES:STRING="${cmr_ANDROID_CPP_FEATURES}" \
    -Dcmr_HOST_BUILD_DIR:PATH=${cmr_HOST_BUILD_DIR} \

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT}
  fi
fi

if [[ ${cmr_TARGET_OS} == "iOS" ]] ; then
  echo "==== Debug: Build Project for iOS"

  ${cmr_CMAKE_CMD} ${cmr_REPO_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=OFF \
    -DBUILD_TESTING:BOOL=${cmr_BUILD_TESTING} \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=${cmr_CMAKE_BUILD_TYPE} \
      -DBUILD_SHARED_LIBS:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \
      -DCMAKE_CONFIGURATION_TYPES:STRING="${cmr_CMAKE_BUILD_TYPE}" \
    -DCMAKE_TOOLCHAIN_FILE:PATH=${cmr_REPO_DIR}/libs/LibCMaker/cmake/ios.toolchain.cmake \
      -DPLATFORM:STRING="SIMULATOR64" \
      -DENABLE_VISIBILITY:BOOL=${cmr_BUILD_SHARED_LIBS} \
    -Dcmr_HOST_BUILD_DIR:PATH=${cmr_HOST_BUILD_DIR} \
    -Dcmr_XCODE_GENERATOR_VERBOSITY_LEVEL:STRING="-quiet" \

    # The job exceeded the maximum log length, and has been terminated.
    #-Dcmr_XCODE_GENERATOR_VERBOSITY_LEVEL:STRING="-quiet"

  if [[ ${TRAVIS_BUILD_STAGE_NAME} == "Build Project" ]] ; then
    ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT} --config ${cmr_CMAKE_BUILD_TYPE}
  fi
fi

echo "==== Debug: End build_project.sh"
