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

#include "Fg/Util/FileUtil.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iterator>
#include <vector>

namespace fg
{
namespace util
{
// Writing the buffer to a .PPM file, assuming it has RGB-structure, one byte
// per color component.
// http://www.antigrain.com/doc/basic_renderers/basic_renderers.agdoc.html
bool writePpmFile(const unsigned char* buf,
    unsigned width,
    unsigned height,
    unsigned bytePerPixel,
    const fg::filesystem::path& fileName)
{
  FILE* fd = fopen(fileName.c_str(), "wb");
  if(fd) {
    fprintf(fd, "P6 %d %d 255 ", width, height);

    unsigned int size = width * height * bytePerPixel;
    for(unsigned int i = 0; i < size; i += bytePerPixel) {
      fwrite(buf + i, 1, 3, fd);
    }

    fclose(fd);
    return true;
  }
  return false;
}

//https://stackoverflow.com/a/37575457
bool compareFiles(const fg::filesystem::path& filePath1,
    const fg::filesystem::path& filePath2)
{
  std::ifstream f1(filePath1.string(), std::ifstream::binary | std::ifstream::ate);
  std::ifstream f2(filePath2.string(), std::ifstream::binary | std::ifstream::ate);

  if(f1.fail() || f2.fail()) {
    return false;  // File problem.
  }

  if(f1.tellg() != f2.tellg()) {
    return false;  // Size mismatch.
  }

  // Seek back to beginning and use std::equal to compare contents.
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
      std::istreambuf_iterator<char>(),
      std::istreambuf_iterator<char>(f2.rdbuf()));
}

// https://stackoverflow.com/a/525103
// https://stackoverflow.com/a/43009155
std::string readFile(const fg::filesystem::path& fileName)
{
  std::ifstream ifs(fileName.string(), std::ios::in | std::ios::binary | std::ios::ate);

  std::ifstream::pos_type fileSize = ifs.tellg();
  if(fileSize < 0)
    return std::string();

  ifs.seekg(0, std::ios::beg);
  std::vector<char> bytes(fileSize);
  ifs.read(&bytes[0], fileSize);

  return std::string(&bytes[0], fileSize);
}

}  // namespace util
}  // namespace fg