@goto license_header
# ****************************************************************************
#  Project:  HtmlGrapheas
#  Purpose:  HTML text editor library
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2018 NikitaFeodonit
#
#    This file is part of the HtmlGrapheas project.
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
:license_header

@rem https://stackoverflow.com/a/35580404
@rem "-B" - specifies path to the build folder
@rem "-H" - specifies path to the source folder

@set MINGW_HOME=\msys2-x64\mingw32
@rem  @set MINGW_HOME=\msys2-x64\mingw64

@set PATH=%MINGW_HOME%\bin;%PATH%

@set CMAKE_CMD="cmake"

@set BUILD_TYPE=Debug
@rem  @set BUILD_TYPE=Release

@set SOURCE_DIR=%~dp0
@set BUILD_DIR=%~dp0build_wx_mingw_cmd\%BUILD_TYPE%


@set ATTACH_WX_CONSOLE=ON


@rem  With "-lmsvcr*" tests crash.

@rem %CMAKE_CMD% --debug-output --trace-expand ^
%CMAKE_CMD% ^
 ^
 -H%SOURCE_DIR% ^
 -B%BUILD_DIR% ^
 ^
 -G "MinGW Makefiles" ^
 ^
 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
 ^
 -DATTACH_WX_CONSOLE=%ATTACH_WX_CONSOLE% ^
 -DBUILD_TESTING=ON ^
 ^
 -DCMAKE_C_STANDARD_LIBRARIES="-lmsvcr110 -static" ^
 -DCMAKE_CXX_STANDARD_LIBRARIES="-lmsvcr110 -static" ^
 ^
 -Dcmr_PRINT_DEBUG=OFF ^
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ^
 -DCMAKE_EXPORT_COMPILE_COMMANDS=OFF ^
 ^
 -DCMAKE_COLOR_MAKEFILE=ON ^
 ^
 && %CMAKE_CMD% --build %BUILD_DIR% -- -j6 ^
 && %CMAKE_CMD% -E env CTEST_OUTPUT_ON_FAILURE=1 ^
    %CMAKE_CMD% --build %BUILD_DIR% --target test


@rem  -DCMAKE_C_STANDARD_LIBRARIES="-static" ^
@rem  -DCMAKE_CXX_STANDARD_LIBRARIES="-static" ^

@rem  -DCMAKE_INSTALL_PREFIX=inst \
