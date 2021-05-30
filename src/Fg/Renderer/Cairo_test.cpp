/*****************************************************************************
 *  Project:  Feographia
 *  Purpose:  The application to work with the biblical text
 *  Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *    Copyright (c) 2017-2021 NikitaFeodonit
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

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"
#include "Poco/Path.h"

#pragma warning(pop)

#include "gtest/gtest.h"

#include "Fg/Renderer/Cairo.h"
#include "Fg/Util/FileUtil.h"

extern Poco::Path testDir;
extern Poco::Path dataDir;

TEST(CairoTest, CairoTest)
{
  //////// Init part

  EXPECT_TRUE(Poco::File {testDir}.exists());
  EXPECT_TRUE(Poco::File {dataDir}.exists());

  //// Cairo init

  enum
  {
    BYTES_PER_PIXEL = 4
  };

  const cairo_format_t colorFormat = CAIRO_FORMAT_RGB24;
  const int frameWidth = 150;
  const int frameHeight = 100;
  //const int stride = frameWidth * BYTES_PER_PIXEL;
  const int stride = cairo_format_stride_for_width(colorFormat, frameWidth);

  std::vector<unsigned char> frameBuf(
      static_cast<unsigned int>(std::abs(stride * frameHeight)));

  fg::CairoPtr cairo = std::make_shared<fg::Cairo>(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride);
  cairo->clear(fg::Color {1, 1, 1, 1});

  //////// Test Cairo::drawLine()

  fg::Color color(0.5, 0.5, 0.5, 1);
  cairo->drawLine(10, 30, 140, 30, 5, color);
  cairo->drawLine(30, 10, 30, 90, 15, color);
  cairo->drawLine(10, 80, 140, 10, 1, color);

  // Write our picture to file.
  std::string fileName1 = "CairoTest_1.ppm";
  Poco::File fileOutTest1 {Poco::Path {testDir}.setFileName(fileName1)};
  fg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1);

  // Compare our file with prototype.
  Poco::File fileTest1 {Poco::Path {dataDir}.setFileName(fileName1)};
  EXPECT_TRUE(fg::util::compareFiles(fileTest1, fileOutTest1));
}
