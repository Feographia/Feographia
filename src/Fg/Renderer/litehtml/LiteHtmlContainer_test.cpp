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

#include <string>

#include "gtest/gtest.h"

#include "Fg/Renderer/Cairo.h"
#include "Fg/Renderer/litehtml/LiteHtmlContainer.h"
#include "Fg/Util/FileUtil.h"
#include "Fg/Util/Filesystem.h"

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

TEST(ContainerTest, create_font)
{
  EXPECT_TRUE(fg::filesystem::exists(testDir));
  EXPECT_TRUE(fg::filesystem::exists(fontDir));

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

  ////

  fg::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(fg::filesystem::exists(fontConfFile));
  std::string fontConfig = fg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

  litehtml::font_metrics fm;
  fg::LiteHtmlContainer container{};
  container.setFontTextCacheSize(1000);
  EXPECT_TRUE(container.parseAndLoadFontConfigFromMemory(fontConfig, true));
  EXPECT_TRUE(container.addFontDir(fontDir));
  litehtml::uint_ptr hFont = container.create_font(
      "Tinos", 16, 400, litehtml::font_style::fontStyleNormal, 0, &fm);
  EXPECT_TRUE(hFont);

  EXPECT_EQ(fm.ascent, 15);
  EXPECT_EQ(fm.descent, -4);
  EXPECT_EQ(fm.height, 18);
  EXPECT_EQ(fm.x_height, 8);
  EXPECT_FALSE(fm.draw_spaces);

  EXPECT_EQ(container.text_width("This is some english text", hFont), 158);
  EXPECT_EQ(container.text_width("some english", hFont), 84);

  container.delete_font(hFont);
}

TEST(ContainerTest, draw_text)
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

  //// HtmlGrapheasKamva init.

  litehtml::font_metrics fm;
  fg::LiteHtmlContainer container{};
  container.setFontTextCacheSize(1000);
  EXPECT_TRUE(container.addFontDir(fontDir));
  // FIXME: The test is failed with font size 16 on Android.
  litehtml::uint_ptr hFont = container.create_font("Tinos", 17, 400,
      litehtml::font_style::fontStyleNormal,
      litehtml::font_decoration_linethrough
          | litehtml::font_decoration_underline,
      &fm);
  EXPECT_TRUE(hFont);

  //////// Test Container::draw_text().

  // Set text position.
  int x = 10;
  int y = frameHeight - 10;
  litehtml::position pos(x, y, 0, 0);

  // Set text color.
  litehtml::web_color textColor(128, 128, 128);

  // draw_text()
  container.draw_text(reinterpret_cast<litehtml::uint_ptr>(&cairo),
      "This is some english text", hFont, textColor, pos);

  // Write our picture to file.
  std::string fileName1 = "LiteHtmlContainer_1.ppm";
  fg::filesystem::path fileOutTest1 = testDir / fileName1;
  fg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1);

  // Compare our file with prototype.
  fg::filesystem::path fileTest1 = dataDir / fileName1;
  EXPECT_TRUE(fg::util::compareFiles(fileTest1, fileOutTest1));

  //////// Repeat tests for new text.

  // Make cleaning before new text.
  cairo->clear(fg::Color{0, 0, 0, 1});

  // draw_text()
  container.draw_text(reinterpret_cast<litehtml::uint_ptr>(&cairo),
      "some english", hFont, textColor, pos);

  // Write our picture to file.
  std::string fileName2 = "LiteHtmlContainer_2.ppm";
  fg::filesystem::path fileOutTest2 = testDir / fileName2;
  fg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest2);

  // Compare our file with prototype.
  fg::filesystem::path fileTest2 = dataDir / fileName2;
  EXPECT_TRUE(fg::util::compareFiles(fileTest2, fileOutTest2));

  //////// Deinit part.

  container.delete_font(hFont);
}

TEST(ContainerTest, drawHtmlDocument)
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
  const unsigned int frameWidth = 640;
  const unsigned int frameHeight = 480;
  //const int stride = frameWidth * BYTES_PER_PIXEL;
  const int stride = cairo_format_stride_for_width(colorFormat, frameWidth);

  std::vector<unsigned char> frameBuf(stride * frameHeight);

  fg::CairoPtr cairo = std::make_shared<fg::Cairo>(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride);
  cairo->clear(fg::Color{1, 1, 1, 1});

  //// HtmlGrapheasKamva init.

  fg::LiteHtmlContainer container{};

  // Set fonts.
  container.setFontTextCacheSize(1000);
  container.setDefaultFontName("Tinos");
  container.setDefaultFontSize(24);
  EXPECT_TRUE(container.addFontDir(fontDir));

  // Set device parameters.
  container.setDeviceWidth(frameWidth);
  container.setDeviceHeight(frameHeight);
  container.setDeviceDpiX(96);
  container.setDeviceDpiY(96);
  container.setDisplayAreaWidth(frameWidth);
  container.setDisplayAreaHeight(frameHeight);
  container.setDeviceMonochromeBits(0);
  container.setDeviceColorBits(8);
  container.setDeviceColorIndex(256);
  container.setDeviceMediaType(litehtml::media_type_screen);

  //////// Draw HTML document.

  // Load master CSS.
  std::string masterCss = fg::util::readFile(dataDir / "master.css");
  EXPECT_FALSE(masterCss.empty());

  litehtml::context htmlContext;
  htmlContext.load_master_stylesheet(masterCss.c_str());

  // Create HTML document from UTF8 string.
  std::string htmlText =
      u8R"(
        <html>
          <body>
            <center><h1>Tile text</h1></center>
            <center><h3>Subtitle text</h3></center>
            This is <i>some</i> <b>English</b> <b><i>text</i></b>
            <blockquote>for HTML renderer test.</blockquote>
          </body>
        </html>
      )";
  litehtml::document::ptr htmlDocument =
      litehtml::document::createFromUTF8(htmlText.c_str(), &container,
          &htmlContext /*, litehtml::css* user_styles = 0 */);

  // Render HTML document.
  int bestWidth = htmlDocument->render(frameWidth);
  EXPECT_GE(bestWidth, 0);

  // Draw HTML document.
  litehtml::position clip(0, 0, frameWidth, frameHeight);
  htmlDocument->draw(reinterpret_cast<litehtml::uint_ptr>(&cairo), 0, 0, &clip);

  // Write our picture to file.
  std::string fileName1 = "LiteHtmlDocument_1.ppm";
  fg::filesystem::path fileOutTest1 = testDir / fileName1;
  fg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1);

  // Compare our file with prototype.
  fg::filesystem::path fileTest1 = dataDir / fileName1;
  EXPECT_TRUE(fg::util::compareFiles(fileTest1, fileOutTest1));
}
