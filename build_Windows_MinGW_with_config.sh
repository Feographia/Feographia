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

MINGW_HOME=/mingw32
#MINGW_HOME=/mingw64

PATH=${MINGW_HOME}/bin:${PATH}

CMAKE_CMD="cmake"

BUILD_TYPE=Release

SOURCE_DIR=./
BUILD_DIR=./build_wx_mingw_msys/${BUILD_TYPE}


ATTACH_WX_CONSOLE=ON


# With "-lmsvcr*" tests crash.

#${CMAKE_CMD} --debug-output --trace-expand \
${CMAKE_CMD} \
\
    -H${SOURCE_DIR} \
    -B${BUILD_DIR} \
\
    -G "MSYS Makefiles" \
\
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
\
    -DATTACH_WX_CONSOLE=${ATTACH_WX_CONSOLE} \
    -DBUILD_TESTING=ON \
\
    -DCMAKE_C_STANDARD_LIBRARIES="-lmsvcr110 -static" \
    -DCMAKE_CXX_STANDARD_LIBRARIES="-lmsvcr110 -static" \
\
    -Dcmr_PRINT_DEBUG=OFF \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=OFF \
\
    -DCMAKE_COLOR_MAKEFILE=ON \
\
    && ${CMAKE_CMD} --build ${BUILD_DIR} -- -j6 \
    && ${CMAKE_CMD} -E env CTEST_OUTPUT_ON_FAILURE=1 \
       ${CMAKE_CMD} --build ${BUILD_DIR} --target test


#    -DCMAKE_C_STANDARD_LIBRARIES="-static" \
#    -DCMAKE_CXX_STANDARD_LIBRARIES="-static" \

#    -DCMAKE_INSTALL_PREFIX=inst \
