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

#include <memory>
#include <string>
#include <vector>

#include <cairo/cairo.h>

#include "gtest/gtest.h"

#include "Fg/Renderer/Cairo.h"
#include "Fg/Renderer/FontLibrary/Font.h"
#include "Fg/Renderer/FontLibrary/FontLibrary.h"
#include "Fg/Util/FileUtil.h"
#include "Fg/Util/Filesystem.h"
#include "Fg/Util/StringUtil.h"

inline fg::filesystem::path testDir;
inline fg::filesystem::path fontDir;
inline fg::filesystem::path dataDir;

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  // https://stackoverflow.com/a/55579815
  testDir =
      fg::filesystem::absolute(fg::filesystem::path(argv[0]).parent_path());
  fontDir = testDir / "fonts";
  dataDir = testDir / "data";

  return RUN_ALL_TESTS();
}

TEST(FontTest, FontTest)
{
  //////// Init part.

  EXPECT_TRUE(fg::filesystem::exists(testDir));
  EXPECT_TRUE(fg::filesystem::exists(fontDir));
  EXPECT_TRUE(fg::filesystem::exists(dataDir));

  //// Cairo init.

  enum
  {
    //BYTES_PER_PIXEL = 3
    BYTES_PER_PIXEL = 4
  };

  const cairo_format_t colorFormat = CAIRO_FORMAT_ARGB32;
  const unsigned int frameWidth = 250;
  const unsigned int frameHeight = 50;
  //const int stride = frameWidth * BYTES_PER_PIXEL;
  const int stride = cairo_format_stride_for_width(colorFormat, frameWidth);

  std::vector<unsigned char> frameBuf(stride * frameHeight);

  fg::CairoPtr cairo = std::make_shared<fg::Cairo>(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride);
  cairo->clear(fg::Color{0, 0, 0, 1});

  //// Init.

  // Init FontLibrary.
  fg::FontLibrary fontLibrary;

  fg::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(fg::filesystem::exists(fontConfFile));
  std::string fontConfig = fg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

  EXPECT_TRUE(fontLibrary.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(fontLibrary.addFontDir(fontDir));

  // NOTE: px = pt * DPI / 72
  int pixelSize = 16;
  int weight = 400;
  fg::FontStyle fontStyle = fg::FontStyle::fontStyleNormal;

  uint_least8_t result;
  fg::filesystem::path filePath = fontLibrary.getFontFilePath(
      {"Tinos"}, pixelSize, weight, fontStyle, &result);
  EXPECT_EQ(fg::FontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Regular.ttf");

  // Create Font with FontLibrary.
  fg::Font font(fontLibrary.ftLibrary(), 1000);

  //////// Test Font::createFtFace().

  EXPECT_TRUE(font.createFtFace(filePath, pixelSize));

  //////// Set text for rendering.

  std::string text = "This is some english text";

  //////// Set HarfBuzz params.

  font.setDirection(HB_DIRECTION_LTR);
  font.setScript(HB_SCRIPT_LATIN);
  font.setLanguage("eng");

  //////// Test Font::getBbox().

  fg::Cairo::TextExtentsPtr extents = font.getTextExtents(text);
  EXPECT_DOUBLE_EQ(extents->x_bearing, 0);
  EXPECT_DOUBLE_EQ(extents->y_bearing, -11);
  EXPECT_DOUBLE_EQ(extents->width, 159.765625);
  EXPECT_DOUBLE_EQ(extents->height, 14);
  EXPECT_DOUBLE_EQ(extents->x_advance, 158.765625);
  EXPECT_DOUBLE_EQ(extents->y_advance, 0);

  //////// Test Font::drawText().

  // Set text position.
  int x = 10;
  //int y = 10 + font.getScaledFontExtents().ascent;
  int y = frameHeight - 10 - font.getScaledFontExtents().descent;

  // Set text color.
  fg::Color color(0.5, 0.5, 0.5, 1);

  // drawText()
  font.drawText(cairo, text, x, y, color);

  // Write our picture to file.
  std::string fileName1 = "FontTest_1.ppm";
  fg::filesystem::path fileOutTest1 = testDir / fileName1;
  fg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1);

  // Compare our file with prototype.
  fg::filesystem::path fileTest1 = dataDir / fileName1;
  EXPECT_TRUE(fg::util::compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  text = "some english";

  // Make cleaning before new text.
  font.clearBuffer();
  cairo->clear(fg::Color{0, 0, 0, 1});

  // Set HarfBuzz params.
  font.setDirection(HB_DIRECTION_LTR);
  font.setScript(HB_SCRIPT_LATIN);
  font.setLanguage("eng");

  // getBbox().
  extents = font.getTextExtents(text);
  EXPECT_DOUBLE_EQ(extents->x_bearing, 0);
  EXPECT_DOUBLE_EQ(extents->y_bearing, -11);
  EXPECT_DOUBLE_EQ(extents->width, 84.046875);
  EXPECT_DOUBLE_EQ(extents->height, 14);
  EXPECT_DOUBLE_EQ(extents->x_advance, 84.046875);
  EXPECT_DOUBLE_EQ(extents->y_advance, 0);

  // drawText().
  font.drawText(cairo, text, x, y, color);

  // Write our picture to file.
  std::string fileName2 = "FontTest_2.ppm";
  fg::filesystem::path fileOutTest2 = testDir / fileName2;
  fg::util::writePpmFile(frameBuf.data(), frameWidth, frameHeight,
      BYTES_PER_PIXEL, fileOutTest2.c_str());

  // Compare our file with prototype.
  fg::filesystem::path fileTest2 = dataDir / fileName2;
  EXPECT_TRUE(fg::util::compareFiles(fileTest2, fileOutTest2));

  //////// Test Font::xHeight().

  EXPECT_DOUBLE_EQ(font.xHeight(), 8.0);
}
