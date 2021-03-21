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

#include <cairo/cairo.h>

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"
#include "Poco/Path.h"

#pragma warning(pop)

#include "gtest/gtest.h"

#include "Fg/Renderer/Cairo.h"
#include "Fg/Renderer/litehtml/LiteHtmlRenderer.h"
#include "Fg/Util/FileUtil.h"

extern Poco::Path testDir;
extern Poco::Path fontDir;
extern Poco::Path dataDir;

// Failed configurationns:
//  Windows, WinXP, MSVC, Debug, shared
//  Windows, x64, GCC, MinGW-w64, Debug, shared
//  Android
TEST(HtmlRenderer, DISABLED_drawHtml)
{
  //////// Init part.

  EXPECT_TRUE(Poco::File {testDir}.exists());
  EXPECT_TRUE(Poco::File {fontDir}.exists());
  EXPECT_TRUE(Poco::File {dataDir}.exists());

  //// Cairo init.

  enum
  {
    //BYTES_PER_PIXEL = 3
    BYTES_PER_PIXEL = 4
  };

  const cairo_format_t colorFormat = CAIRO_FORMAT_ARGB32;
  const int frameWidth = 640;
  const int frameHeight = 480;
  //const int stride = frameWidth * BYTES_PER_PIXEL;
  const int stride = cairo_format_stride_for_width(colorFormat, frameWidth);

  std::vector<unsigned char> frameBuf(
      static_cast<unsigned int>(std::abs(stride * frameHeight)));

  fg::CairoPtr cairo = std::make_shared<fg::Cairo>(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride);
  cairo->clear(fg::Color {1, 1, 1, 1});

  //// HtmlGrapheasKamva init.

  fg::LiteHtmlRenderer htmlRenderer;
  fg::LiteHtmlContainerPtr htmlContainer = htmlRenderer.getContainer();

  // Load font config.
  Poco::File fontConfFile {Poco::Path {fontDir}.setFileName("fonts.conf")};
  EXPECT_TRUE(fontConfFile.exists());
  std::string fontConfig = fg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

//  bool loadedFontConfig =
//      htmlContainer->parseAndLoadFontConfigFromMemory(fontConfig, true);
//  assert(loadedFontConfig);

  // Set fonts.
  htmlContainer->setFontTextCacheSize(1000);
  htmlContainer->setDefaultFontName("Tinos");
  htmlContainer->setDefaultFontSize(24);
  EXPECT_TRUE(htmlContainer->addFontDir(fontDir));

  // Set device parameters.
  htmlContainer->setDeviceDpiX(96);
  htmlContainer->setDeviceDpiY(96);
  htmlContainer->setDeviceMonochromeBits(0);
  htmlContainer->setDeviceColorBits(8);
  htmlContainer->setDeviceColorIndex(256);
  htmlContainer->setDeviceMediaType(litehtml::media_type_screen);

  //////// Draw HTML document.

  // Load master CSS.
  Poco::File masterCssFile {Poco::Path {dataDir}.setFileName("master.css")};
  std::string masterCss = fg::util::readFile(masterCssFile);
  EXPECT_GE(masterCss.size(), 0);

  htmlRenderer.getHtmlContext()->load_master_stylesheet(masterCss.c_str());

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
  htmlRenderer.createHtmlDocumentFromUtf8(htmlText);

  // Render HTML document.
  int bestWidth = htmlRenderer.renderHtml(frameWidth, frameHeight);
  EXPECT_GE(bestWidth, 0);

  // Set background color.
  htmlRenderer.setBackgroundColor(litehtml::web_color(255, 255, 255));

  // Draw HTML document. Test the text scrolling.

  //  htmlRenderer.drawHtml(
  //      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 0, 207);
  //  htmlRenderer.drawHtml(
  //      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 0, -217);
  //  htmlRenderer.drawHtml(
  //      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 0, 0);

  //  htmlRenderer.drawHtml(
  //      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 205, 0);
  //  htmlRenderer.drawHtml(
  //      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, -304, 0);
  //  htmlRenderer.drawHtml(
  //      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 0, 0);

  htmlRenderer.drawHtml(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 205, 207);
  htmlRenderer.drawHtml(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, -304,
      -217);
  htmlRenderer.drawHtml(
      frameBuf.data(), colorFormat, frameWidth, frameHeight, stride, 0, 0);

  // Write our picture to file.
  std::string fileName1 = "LiteHtmlDocument_1.ppm";
  Poco::File fileOutTest1 {Poco::Path {testDir}.setFileName(fileName1)};
  fg::util::writePpmFile(
      frameBuf.data(), frameWidth, frameHeight, BYTES_PER_PIXEL, fileOutTest1);

  // Compare our file with prototype.
  Poco::File fileTest1 {Poco::Path {dataDir}.setFileName(fileName1)};
  EXPECT_TRUE(fg::util::compareFiles(fileTest1, fileOutTest1));
}
