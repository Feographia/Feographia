@goto license_header
# ****************************************************************************
#  Project:  Feographia
#  Purpose:  The application to work with the biblical text
#  Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#    Copyright (c) 2017-2018 NikitaFeodonit
#
#    This file is part of the Feographia project.
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

@rem https://stackoverflow.com/a/25746667
@rem -T v120_xp  (the best)
@rem or
@rem -DCMAKE_GENERATOR_TOOLSET=v120_xp

@rem https://stackoverflow.com/a/20423820
@rem To change the build type, on Windows, it must be done at build time:
@rem cmake --build {DIR} --config Release

@rem https://stackoverflow.com/a/35580404
@rem "-B" - specifies path to the build folder
@rem "-H" - specifies path to the source folder

@rem CMake 3.7 is minimum for generator "Visual Studio 15 2017".
@set CMAKE_CMD="cmake"


@set BUILD_TYPE=Debug
@rem set BUILD_TYPE=Release

@set SOURCE_DIR=%~dp0
@set BUILD_DIR=%~dp0build_wx_msvc


@rem TODO: add prefix 'HG_' to all vars, which is defined project.
@set ATTACH_WX_CONSOLE=ON


@if "%1" == "" goto win64
@if not "%2" == "" goto usage

@if /i %1 == win32_xp  goto win32_xp
@if /i %1 == win32     goto win32
@if /i %1 == win64     goto win64
@if /i %1 == arm       goto arm
@goto usage


:win32_xp
@set VS_GENERATORERATOR=-DCMAKE_GENERATOR="Visual Studio 15 2017"
@set VS_PLATFORM=
@set VS_TOOLSET=-DCMAKE_GENERATOR_TOOLSET="v141_xp"
@goto :RunCMake

:win32
@set VS_GENERATOR=-DCMAKE_GENERATOR="Visual Studio 15 2017"
@set VS_PLATFORM=
@set VS_TOOLSET=
@goto :RunCMake

:win64
@set VS_GENERATOR=-DCMAKE_GENERATOR="Visual Studio 15 2017"
@set VS_PLATFORM=-DCMAKE_GENERATOR_PLATFORM=x64
@set VS_TOOLSET=
@goto :RunCMake

:arm
@set VS_GENERATOR=-DCMAKE_GENERATOR="Visual Studio 15 2017"
@set VS_PLATFORM=-DCMAKE_GENERATOR_PLATFORM=ARM
@set VS_TOOLSET=
@goto :RunCMake


:RunCMake
@rem %CMAKE_CMD% --debug-output --trace-expand ^
%CMAKE_CMD% ^
 ^
 -H%SOURCE_DIR% ^
 -B%BUILD_DIR% ^
 ^
 %VS_GENERATOR% ^
 %VS_PLATFORM% ^
 %VS_TOOLSET% ^
 ^
 -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
 -DCMAKE_CONFIGURATION_TYPES=%BUILD_TYPE% ^
 ^
 -DATTACH_WX_CONSOLE=%ATTACH_WX_CONSOLE% ^
 -DBUILD_TESTING=ON ^
 ^
 -Dcmr_PRINT_DEBUG=OFF ^
 -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF ^
 -DCMAKE_EXPORT_COMPILE_COMMANDS=OFF ^
 ^
 -DCMAKE_COLOR_MAKEFILE=ON ^
 ^
 && %CMAKE_CMD% --build %BUILD_DIR% --config %BUILD_TYPE% ^
 && %CMAKE_CMD% -E env CTEST_OUTPUT_ON_FAILURE=1 ^
    %CMAKE_CMD% --build %BUILD_DIR% --config %BUILD_TYPE% --target RUN_TESTS

@goto :eof

:usage
@echo usage TODO
@goto :eof
