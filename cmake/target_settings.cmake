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

#-----------------------------------------------------------------------
# Settigs for iOS Bundle
#-----------------------------------------------------------------------

if(IOS)
  option(IOS_DISABLE_CODESIGN "IOS_DISABLE_CODESIGN" ON)


  # https://help.apple.com/xcode/mac/8.0/#/itcaec37c2a6
  # Build settings reference
  # google search string: "build setting reference" site:developer.apple.com
  #
  # https://stackoverflow.com/questions/6910901/how-do-i-print-a-list-of-build-settings-in-xcode-project
  # "How do I print a list of .Build Settings. in Xcode project?"
  #
  # $ xcodebuild -project myProj.xcodeproj -target "myTarg" -showBuildSettings

  # "Building iOS applications using xcodebuild without codesign"
  # https://stackoverflow.com/a/58451900
  # https://stackoverflow.com/a/11647504
  # https://stackoverflow.com/a/39901677
  #
  # CODE_SIGN_IDENTITY=""
  # CODE_SIGNING_REQUIRED="NO"
  # CODE_SIGN_ENTITLEMENTS=""
  # CODE_SIGNING_ALLOWED="NO"


  # From https://github.com/ruslo/sugar
  #set(CMAKE_MACOSX_BUNDLE YES)
  #set(CMAKE_OSX_SYSROOT "iphoneos")
  #set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
  #set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
  #set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.example.sugar.iOSGTest")

  # From https://stackoverflow.com/a/972579
  #set(CMAKE_OSX_SYSROOT iphoneos2.2.1)
  #set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
  #set(CMAKE_CXX_FLAGS "-x objective-c++")
  #set(CMAKE_EXE_LINKER_FLAGS "-framework AudioToolbox -framework CoreGraphics -framework QuartzCore -framework UIKit")
  #link_directories(\${HOME}/\${SDKROOT}/lib)
  #set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.mycompany.\${PRODUCT_NAME:identifier}")
  #set(APP_TYPE MACOSX_BUNDLE)


  set(IOS_APP_BUNDLE_IDENTIFIER "ru.feographia.ios_app")

  set(DEVELOPMENT_TEAM_ID "TODO")  # Team ID from Apple.
  set(IOS_CODE_SIGN_IDENTITY "iPhone Developer")
  if(IOS_DISABLE_CODESIGN)
    set(IOS_CODE_SIGN_IDENTITY "")
  endif()

  # NOTE:
  # Link error:
  # "targeted OS version does not support use of thread local variables".
  # C++11 concept not supported all iOS platforms,
  # thread_local is allowed beginning with iOS 9 for Xcode 10.
  # Xcode 9 + iOS 8 compiles OK.
  set(IOS_DEPLOYMENT_TARGET "9.0")  # Deployment target version of iOS.

  # Set to "1" to target iPhone, "2" to target iPad, "1,2" to target both.
  set(IOS_DEVICE_FAMILY "1")

  # See CMake's docs for MACOSX_BUNDLE_INFO_PLIST target property for these vars.
  # See the file MacOSXBundleInfo.plist.in for MACOSX_BUNDLE_EXECUTABLE_NAME.
  set(MACOSX_BUNDLE_EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})
  #set(MACOSX_BUNDLE_INFO_STRING ${IOS_APP_BUNDLE_IDENTIFIER})
  #set(MACOSX_BUNDLE_GUI_IDENTIFIER ${IOS_APP_BUNDLE_IDENTIFIER})
  #set(MACOSX_BUNDLE_BUNDLE_NAME ${IOS_APP_BUNDLE_IDENTIFIER})
  set(MACOSX_BUNDLE_ICON_FILE "")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2017-2020 NikitaFeodonit")
  set(MACOSX_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET})

  #set(CMAKE_MACOSX_BUNDLE ON)
  #set(CMAKE_FRAMEWORK ON)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
  #set(CMAKE_INSTALL_RPATH "@executable_path/Frameworks" "@loader_path/Frameworks")
  set(CMAKE_INSTALL_RPATH "@executable_path/lib" "@loader_path/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

  set(IOS_MACOSX_BUNDLE MACOSX_BUNDLE)  # For add_executable()
endif()
