#!/bin/bash

set -ev

echo "==== Debug: Begin build_host_tools.sh"

${cmr_CMAKE_CMD} --version
cd ${cmr_HOST_BUILD_DIR}

if [[ ${cmr_TARGET_OS} == "Android" ]] ; then
  echo "==== Debug: Build Host Tools for Android"

  ${cmr_CMAKE_CMD} ${cmr_HOST_TOOLS_PROJECT_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=ON \
    -DBUILD_TESTING:BOOL=OFF \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_HOST_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_HOST_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=Release \
      -DBUILD_SHARED_LIBS:BOOL=OFF \
    -DFgMainProject_DIR:PATH=${cmr_REPO_DIR} \
      -DLibCMaker_LIB_DIR:PATH=${cmr_REPO_DIR}/libs \
      -DHOST_TOOLS_STAMP_FILE_NAME:STRING=${cmr_HOST_TOOLS_STAMP_FILE_NAME} \

  ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT}
fi

if [[ ${cmr_TARGET_OS} == "iOS" ]] ; then
  echo "==== Debug: Build Host Tools for iOS"

  ${cmr_CMAKE_CMD} ${cmr_HOST_TOOLS_PROJECT_DIR} \
    -Dcmr_BUILD_MULTIPROC_CNT:STRING=${cmr_JOBS_CNT} \
    -Dcmr_PRINT_DEBUG:BOOL=ON \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_COLOR_MAKEFILE:BOOL=ON \
    -DBUILD_TESTING:BOOL=OFF \
    -DCMAKE_INSTALL_PREFIX:PATH=${cmr_HOST_INSTALL_DIR} \
    -Dcmr_DOWNLOAD_DIR:PATH=${cmr_DOWNLOAD_DIR} \
    -Dcmr_UNPACKED_DIR:PATH=${cmr_HOST_UNPACKED_DIR} \
      -DCMAKE_BUILD_TYPE:STRING=Release \
      -DBUILD_SHARED_LIBS:BOOL=OFF \
    -DFgMainProject_DIR:PATH=${cmr_REPO_DIR} \
      -DLibCMaker_LIB_DIR:PATH=${cmr_REPO_DIR}/libs \
      -DHOST_TOOLS_STAMP_FILE_NAME:STRING=${cmr_HOST_TOOLS_STAMP_FILE_NAME} \
    -DCMAKE_GENERATOR:STRING="${cmr_CMAKE_GENERATOR}" \
      -DCMAKE_CONFIGURATION_TYPES:STRING=Release \

    #-Dcmr_XCODE_GENERATOR_VERBOSITY_LEVEL:STRING="-quiet" \

  ${cmr_CMAKE_CMD} --build . --parallel ${cmr_JOBS_CNT} --config Release
fi

echo "==== Debug: End build_host_tools.sh"
