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

#include <cstdlib>
#include <string>

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"
#include "Poco/Path.h"

#pragma warning(pop)

#include "gtest/gtest.h"

#include "Fg/Renderer/Font/Case.h"
#include "Fg/Util/FileUtil.h"
#include "Fg/Util/StringUtil.h"

extern Poco::Path testDir;
extern Poco::Path fontDir;
//extern Poco::Path dataDir;

TEST(FontCaseTest, getFontFilePath)
{
  EXPECT_TRUE(Poco::File {testDir}.exists());
  EXPECT_TRUE(Poco::File {fontDir}.exists());
  //EXPECT_TRUE(Poco::File {dataDir}.exists());

  fg::font::Case fontCase;

  Poco::File fontConfFile {Poco::Path {fontDir}.setFileName("fonts.conf")};
  EXPECT_TRUE(fontConfFile.exists());
  std::string fontConfig = fg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

  EXPECT_TRUE(fontCase.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(fontCase.addFontDir(fontDir));

  fg::font::Case::FontMatches result;
  Poco::File filePath;

  filePath = fontCase.getFontFilePath(
      {"Some Unknown Font", "Tinos"}, 16, 400,
      fg::font::Case::FontStyle::fontStyleNormal, &result);
  EXPECT_EQ(fg::font::Case::FontMatches::allMatched, result);
  EXPECT_TRUE(
      Poco::Path {filePath.path()}.getFileName() == "Tinos-Regular.ttf");

  filePath = fontCase.getFontFilePath(
      {"Some Unknown Font", "Tinos"}, 16, 400,
      fg::font::Case::FontStyle::fontStyleItalic, &result);
  EXPECT_EQ(fg::font::Case::FontMatches::allMatched, result);
  EXPECT_TRUE(Poco::Path {filePath.path()}.getFileName() == "Tinos-Italic.ttf");

  filePath = fontCase.getFontFilePath(
      {"Some Unknown Font", "Tinos"}, 16, 700,
      fg::font::Case::FontStyle::fontStyleNormal, &result);
  EXPECT_EQ(fg::font::Case::FontMatches::allMatched, result);
  EXPECT_TRUE(Poco::Path {filePath.path()}.getFileName() == "Tinos-Bold.ttf");

  filePath = fontCase.getFontFilePath(
      {"Some Unknown Font", "Tinos"}, 16, 700,
      fg::font::Case::FontStyle::fontStyleItalic, &result);
  EXPECT_EQ(fg::font::Case::FontMatches::allMatched, result);
  EXPECT_TRUE(
      Poco::Path {filePath.path()}.getFileName() == "Tinos-BoldItalic.ttf");
}
