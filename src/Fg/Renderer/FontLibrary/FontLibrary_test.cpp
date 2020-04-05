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

#include "gtest/gtest.h"

#include "Fg/Renderer/FontLibrary/FontLibrary.h"
#include "Fg/Util/FileUtil.h"
#include "Fg/Util/Filesystem.h"
#include "Fg/Util/StringUtil.h"

inline fg::filesystem::path testDir;
inline fg::filesystem::path fontDir;
//inline fg::filesystem::path dataDir;

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  // https://stackoverflow.com/a/55579815
  // See also:
  // https://stackoverflow.com/a/42772814
  // https://habr.com/ru/post/343622/
  testDir =
      fg::filesystem::absolute(fg::filesystem::path(argv[0]).parent_path());
  fontDir = testDir / "fonts";
  //  dataDir = testDir / "data";

  return RUN_ALL_TESTS();
}

TEST(FontLibraryTest, getFontFilePath)
{
  EXPECT_TRUE(fg::filesystem::exists(testDir));
  EXPECT_TRUE(fg::filesystem::exists(fontDir));
  //  EXPECT_TRUE(fg::filesystem::exists(dataDir));

  fg::FontLibrary fontLibrary;

  fg::filesystem::path fontConfFile = fontDir / "fonts.conf";
  EXPECT_TRUE(fg::filesystem::exists(fontConfFile));
  std::string fontConfig = fg::util::readFile(fontConfFile);
  EXPECT_FALSE(fontConfig.empty());

  EXPECT_TRUE(fontLibrary.parseAndLoadConfigFromMemory(fontConfig, true));

  EXPECT_TRUE(fontLibrary.addFontDir(fontDir));

  uint_least8_t result;
  fg::filesystem::path filePath;

  filePath = fontLibrary.getFontFilePath({"Some Unknown Font", "Tinos"}, 16, 400,
      fg::FontStyle::fontStyleNormal, &result);
  EXPECT_EQ(fg::FontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Regular.ttf");

  filePath = fontLibrary.getFontFilePath({"Some Unknown Font", "Tinos"}, 16, 400,
      fg::FontStyle::fontStyleItalic, &result);
  EXPECT_EQ(fg::FontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Italic.ttf");

  filePath = fontLibrary.getFontFilePath({"Some Unknown Font", "Tinos"}, 16, 700,
      fg::FontStyle::fontStyleNormal, &result);
  EXPECT_EQ(fg::FontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-Bold.ttf");

  filePath = fontLibrary.getFontFilePath({"Some Unknown Font", "Tinos"}, 16, 700,
      fg::FontStyle::fontStyleItalic, &result);
  EXPECT_EQ(fg::FontLibrary::FontMatches::allMatched, result);
  EXPECT_TRUE(filePath.filename() == "Tinos-BoldItalic.ttf");
}
