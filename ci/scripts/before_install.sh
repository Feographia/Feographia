#!/bin/bash

set -ev

echo "==== Debug: Begin before_install.sh"

if [[ ( "${TRAVIS_BUILD_STAGE_NAME}" == "Build Host Tools" ) ||
    ( "${TRAVIS_BUILD_STAGE_NAME}" == "Build Project" &&
      ( ${cmr_TARGET_OS} == "Linux" ||
        ${cmr_TARGET_OS} == "Windows" ||
        ${cmr_TARGET_OS} == "macOS" ) ) ]] ; then

  echo "==== Debug: Stage 'Build Host Tools' or 'Build Project'"
  echo "==== Debug: Init work dir"

  # ==== Clean not persistent cache dirs ====
  # Do not use cache for build, only to send data between stages.
  rm -f -d -R ${cmr_WORK_DIR}
  mkdir -p ${cmr_WORK_DIR}

#  # ==== Add ssh key ====
#  # https://github.com/dwyl/learn-travis/blob/master/encrypted-ssh-keys-deployment.md
#  openssl aes-256-cbc -K ${encrypted_71baf22766bc_key} \
#    -iv ${encrypted_71baf22766bc_iv} \
#    -in ci/TravisCiKey.enc -out ci/TravisCiKey -d
#  chmod 600 ci/TravisCiKey
#  #echo -e "Host $SERVER_IP_ADDRESS\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config
#  if [[ ${cmr_TARGET_OS} == "Windows" ]] ; then
#    ssh -oStrictHostKeyChecking=no github.com uptime
#  fi
#  eval "$(ssh-agent -s)"
#  ssh-add ci/TravisCiKey

  # ==== Clone git repo ====
  echo "==== Debug: Clone git repo"

  cp -r ${cmr_REPO_MAIN_DIR} ${cmr_WORK_DIR}
  cd ${cmr_REPO_DIR}

  # https://gist.github.com/iedemam/9830045
  # https://stackoverflow.com/a/24600210
  # Replace the SSH URL with the public URL.
  # macOS specific sed version required the -e option after the -i option.
  sed -i -e 's/git@github.com:/https:\/\/github.com\//' .gitmodules

  git submodule update --init --recursive

  # ==== Make work dirs ====
  echo "==== Debug: Make work dirs"
  mkdir -p ${cmr_DOWNLOAD_DIR}
  if [[ ! -d ${cmr_INSTALL_DIR} ]] ; then
    mkdir -p ${cmr_INSTALL_DIR}
  fi

  # ==== Install CMake ====
  if [[ ! -f ${cmr_CMAKE_CMD} ]] ; then
    echo "==== Debug: wget CMake"
    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR} ${cmr_CMAKE_BASE_URL}/${cmr_CMAKE_AR_FILE_NAME}
    if [[ ${cmr_TARGET_OS} == "Windows" ]] ; then
      echo "==== Debug: Unpack CMake with 7z"
      7z.exe x -aoa -o${cmr_INSTALL_DIR} ${cmr_DOWNLOAD_DIR}/${cmr_CMAKE_AR_FILE_NAME}
    else
      echo "==== Debug: Unpack CMake with tar"
      tar -xf ${cmr_DOWNLOAD_DIR}/${cmr_CMAKE_AR_FILE_NAME} --directory ${cmr_INSTALL_DIR}
    fi
  fi
fi

if [[ "${TRAVIS_BUILD_STAGE_NAME}" == "Build Host Tools" ]] ; then
  # ==== Make work dirs ====
  echo "==== Debug: Stage 'Build Host Tools'"
  echo "==== Debug: Make work dirs"
  mkdir -p ${cmr_UNPACKED_DIR}
  mkdir -p ${cmr_BUILD_DIR}
  mkdir -p ${cmr_HOST_UNPACKED_DIR}
  mkdir -p ${cmr_HOST_BUILD_DIR}
fi

if [[ "${TRAVIS_BUILD_STAGE_NAME}" == "Build Project" ]] ; then
  echo "==== Debug: Stage 'Build Project'"

  cd ${cmr_REPO_DIR}

  # ==== Make work dirs ====
  echo "==== Debug: Make work dirs"
  mkdir -p ${cmr_DOWNLOAD_DIR}
  mkdir -p ${cmr_UNPACKED_DIR}
  mkdir -p ${cmr_BUILD_DIR}
  mkdir -p ${cmr_NOT_CACHED_INSTALL_DIR}

  # ==== Install MinGW-w64 GCC compiller ====
  # TODO: copy MinGW dll-s to lib-dir and do not install MinGW in "Test Project" stage.
  if [[ ${cmr_TARGET_OS} == "Windows" && ${cmr_MINGW} == "ON" ]] ; then
    echo "==== Debug: Install MinGW-w64"
    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR}\${cmr_MINGW_ARCH_NAME} ${cmr_MINGW_URL}
    7z.exe x -aoa -o${cmr_NOT_CACHED_INSTALL_DIR} ${cmr_DOWNLOAD_DIR}\${cmr_MINGW_ARCH_NAME}
  fi

  # ==== Install Android SDK and NDK ====
  if [[ ${cmr_TARGET_OS} == "Android" ]] ; then
    # ==== Android NDK ====
    echo "==== Debug: Install Android NDK"
    # TODO: copy libc++_shared.so to lib dir and do not install NDK in "Test Project" stage.
    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR} https://dl.google.com/android/repository/${cmr_ANDROID_NDK_PLATFORM}.zip
    unzip -q ${cmr_DOWNLOAD_DIR}/${cmr_ANDROID_NDK_PLATFORM}.zip -d ${cmr_NOT_CACHED_INSTALL_DIR}

    # ==== Android SDK ====
    echo "==== Debug: Install Android SDK"
    # Android SDK is only needed to find the path to the 'adb'
    # in 'test/CMakeLists.txt' 'find_program(adb_exec adb)'

    mkdir -p ${ANDROID_HOME}/cmdline-tools
    # https://stackoverflow.com/a/60598900
    # 'platforms' dir must be, at least empty.
    mkdir -p ${ANDROID_HOME}/platforms

    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR} https://dl.google.com/android/repository/${cmr_ANDROID_CMD_TOOLS_FILE_NAME}
    unzip -q ${cmr_DOWNLOAD_DIR}/${cmr_ANDROID_CMD_TOOLS_FILE_NAME} -d ${ANDROID_HOME}/cmdline-tools

    # https://stackoverflow.com/a/60598900
    yes | sdkmanager --licenses
    sdkmanager "platform-tools"
  fi
fi

if [[ "${TRAVIS_BUILD_STAGE_NAME}" == "Test Project" ]] ; then
  echo "==== Debug: Stage 'Test Project'"

  cd ${cmr_REPO_DIR}

  # ==== Make work dirs ====
  echo "==== Debug: Make work dirs"
  mkdir -p ${cmr_DOWNLOAD_DIR}
  mkdir -p ${cmr_NOT_CACHED_INSTALL_DIR}

  # ==== Install MinGW-w64 GCC compiller ====
  # TODO: copy MinGW dll-s to lib-dir and do not install MinGW if "Test Project" stage.
  if [[ ${cmr_TARGET_OS} == "Windows" && ${cmr_MINGW} == "ON" ]] ; then
    echo "==== Debug: Install MinGW-w64"
    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR}\${cmr_MINGW_ARCH_NAME} ${cmr_MINGW_URL}
    7z.exe x -aoa -o${cmr_NOT_CACHED_INSTALL_DIR} ${cmr_DOWNLOAD_DIR}\${cmr_MINGW_ARCH_NAME}
  fi

  # ==== Install Android SDK with emulator and NDK ====
  if [[ ${cmr_TARGET_OS} == "Android" ]] ; then
    # ==== Android NDK ====
    echo "==== Debug: Install Android NDK"
    # TODO: copy libc++_shared.so to lib dir and do not install NDK in "Test Project" stage.
    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR} https://dl.google.com/android/repository/${cmr_ANDROID_NDK_PLATFORM}.zip
    unzip -q ${cmr_DOWNLOAD_DIR}/${cmr_ANDROID_NDK_PLATFORM}.zip -d ${cmr_NOT_CACHED_INSTALL_DIR}

    # ==== Android SDK ====
    echo "==== Debug: Install Android SDK"
    mkdir -p ${ANDROID_HOME}/cmdline-tools
    # https://stackoverflow.com/a/60598900
    # 'platforms' dir must be, at least empty.
    mkdir -p ${ANDROID_HOME}/platforms

    wget -nv -c -N -P ${cmr_DOWNLOAD_DIR} https://dl.google.com/android/repository/${cmr_ANDROID_CMD_TOOLS_FILE_NAME}
    unzip -q ${cmr_DOWNLOAD_DIR}/${cmr_ANDROID_CMD_TOOLS_FILE_NAME} -d ${ANDROID_HOME}/cmdline-tools

    # https://stackoverflow.com/a/60598900
    yes | sdkmanager --licenses
    sdkmanager "emulator"
    sdkmanager "platform-tools"
    sdkmanager "system-images;android-${cmr_ANDROID_EMULATOR_API_LEVEL};default;${cmr_ANDROID_ABI}"
  fi
fi

echo "==== Debug: End before_install.sh"
