# Feographia

The application to work with the biblical text.


## Requirements

1. The Feographia building requires CMake 3.11+ for "Visual Studio" generator on Windows, CMake 3.12+ for "Xcode" generator on macOS, CMake 3.8+ for "Unix Makefiles" generator on macOS, CMake 3.15+ for "Xcode" generator on iOS and CMake 3.8+ for others.

2. TODO: compilers with C++17 support, Android SDK and NDK versions, Swift tools versions.

3. TODO: OS versions: Linux, Windows (WinXP, Win7+), macOS, Android, iOS.


## Notes

The user interface is made with wxWidgets on Linux, Windows, macOS, with Android SDK on Android and with __TODO__ on iOS.


## Building

Feographia's build system is based on the CMake and CMake build scripts from LibCMaker project.

See the [LibCMaker](https://github.com/LibCMaker/LibCMaker) repository for the documentation, known build issues, build status, etc. of the library dependencies.

At the moment, the assembly and testing of Feographia are making for Linux, Windows, Android, macOS and iOS.

To get the sources:
```
git clone --recursive https://github.com/Feographia/Feographia.git
```

The compile commands for the different platforms see in the
```
Feographia/.travis.yml
Feographia/.appveyor.yml
```

All library dependency source codes are loaded during the CMake configuration phase by the LibCMaker's scripts.


## CI build configurations

Feographia is tested with Travis CI and AppVeyor.

Following configurations are in the Travis CI (2020-05-24):

1. Linux -- Ubuntu Bionic 18.04.4, CMake 3.17.2, Make, Matrix: [GCC 9.3.0 | Clang 9.0.0], [Debug shared | Release static].

2. Windows -- Windows Server 1809, CMake 3.17.2, MSVC 2017 (MSVC 19.16.27035.0), MinGW-w64 GCC 9.3.0 (winlibs-x86_64-posix-seh-gcc-9.3.0-llvm-10.0.0-mingw-w64-7.0.0-r4), Matrix: [MSVC x64 | MSVC Win32 | MSVC WinXP | MinGW-w64 x64], [Debug shared | Release static].

3. macOS -- Mac OS X 10.14.4, CMake 3.17.2, Xcode 10.2.1, Apple Clang 10.0.1, Matrix: CMake generator ["Xcode" | "Unix Makefiles"], [Debug shared | Release static].

4. Android -- Ubuntu Bionic 18.04.4, CMake 3.17.2, Android NDK r21b, Clang 9.0.8, Ninja, Release, Matrix: [Debug shared + c++_shared | Release static + c++_static], [armeabi-v7a + API 16 | arm64-v8a + API 21 | x86 + API 16 | x86_64 + API 21].

5. iOS -- Mac OS X 10.14.4, CMake 3.17.2, Xcode 10.2.1, Apple Clang 10.0.1, SDK iPhoneSimulator12.2, platform SIMULATOR64, CMake generator "Xcode", Matrix: [Debug shared | Release static].

[Disabled, used Travis CI] Following configurations are in the AppVeyor (2019-02-23):

1. Windows 10.0.14393, CMake 3.11.0, MSVC 2017, MSVC 19.16.27026.1, Release, Matrix: [x64 | x32 | WinXP], [shared | static].

2. Windows 10.0.14393, CMake 3.8.0, MinGW-w64 x86_64-7.2.0-posix-seh-rt_v5-rev1, GCC GNU 7.2.0, Release, x64, Matrix: [shared | static].

3. Windows 6.3.9600, CMake 3.11.0, MSVC 2015, MSVC 19.0.24241.7, Release, x64, Matrix: [shared | static].


## CI build status

 *Application*   | *Travis CI* <br> Linux <br> Windows <br> macOS <br> Android <br> iOS   | *Built with dependencies*
 --------------- | ---------------------------------------------------------------------- | ---------------------------
 [Feographia](https://github.com/Feographia/Feographia) <br> [Feographia's site](https://github.com/Feographia/Feographia) | [![Build Status](https://travis-ci.com/Feographia/Feographia.svg?branch=master)](https://travis-ci.com/Feographia/Feographia) | GTest, Dirent (only for Windows), zlib, libpng, fmt, spdlog, STLCache, Expat, ICU, SQLite3, SQLiteModernCPP, Boost (macOS, Android, iOS), HarfBuzz, FreeType, FontConfig, Pixman, Cairo, gumbo (embedded into litehtml), litehtml, wxWidgets (Linux, Windows, macOS)
