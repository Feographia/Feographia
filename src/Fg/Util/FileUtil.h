/*****************************************************************************
 *  Project:  Feographia
 *  Purpose:  The application to work with the biblical text
 *  Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *    Copyright (c) 2017-2020 NikitaFeodonit
 *
 *    This file is part of the Feographia project.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published
 *    by the Free Software Foundation, either version 3 of the License,
 *    or (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *    See the GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program. If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef FG_FILEUTIL_H
#define FG_FILEUTIL_H

#include <string>

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"

#pragma warning(pop)

namespace fg
{
namespace util
{
bool writePpmFile(
    const unsigned char* buf,
    unsigned width,
    unsigned height,
    unsigned bytePerPixel,
    const Poco::File& file);

bool compareFiles(const Poco::File& file1, const Poco::File& file2);

std::string readFile(const Poco::File& file);

}  // namespace util
}  // namespace fg

#endif  // FG_FILEUTIL_H
