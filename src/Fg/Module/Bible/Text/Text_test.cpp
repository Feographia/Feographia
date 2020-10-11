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

#include "gtest/gtest.h"

#include <iostream>
#include <sstream>
#include <utility>

#include <sqlite_modern_cpp.h>

#include "Fg/Module/Bible/Text/Text.h"
#include "Fg/Util/Filesystem.h"

extern fg::filesystem::path testDir;

TEST(TextTest, sqlCreateModule)
{
  try {
    EXPECT_TRUE(fg::filesystem::exists(testDir));

    fg::filesystem::path moduleFile = testDir / "testModule.SQLite3";
    if(fg::filesystem::exists(moduleFile)) {
      fg::filesystem::remove(moduleFile);
    }
    EXPECT_FALSE(fg::filesystem::exists(moduleFile));

    {
      std::vector<fg::Word> words;

      // In the beginning God created the heaven and the earth.
      words.emplace_back(fg::ItemId {1, 1, 1, 1}, "In");
      words.emplace_back(fg::ItemId {1, 1, 1, 2}, "the");
      words.emplace_back(fg::ItemId {1, 1, 1, 3}, "beginning");
      words.emplace_back(fg::ItemId {1, 1, 1, 4}, "God");
      words.emplace_back(fg::ItemId {1, 1, 1, 5}, "created");
      words.emplace_back(fg::ItemId {1, 1, 1, 6}, "the");
      words.emplace_back(fg::ItemId {1, 1, 1, 7}, "heaven");
      words.emplace_back(fg::ItemId {1, 1, 1, 8}, "and");
      words.emplace_back(fg::ItemId {1, 1, 1, 9}, "the");
      words.emplace_back(fg::ItemId {1, 1, 1, 10}, "earth.");

      fg::WordTextId id = (1L << 48) + (1L << 32) + (1L << 16) + 1L;
      std::stringstream ss;
      for(const auto& word : words) {
        EXPECT_EQ(word.getId().getWordTextId(), id++);
        EXPECT_EQ(word.getUuid(), 0);
        ss << word.getText() << " ";
      }
      EXPECT_STREQ(
          ss.str().c_str(),
          "In the beginning God created the heaven and the earth. ");

      fg::Text module {moduleFile};
      module.sqlCreateDatabase();
      module.sqlCreateTable();
      module.sqlInsertWords(words);
      module.sqlCloseDatabase();
    }

    {
      fg::Text module {moduleFile};
      module.sqlOpenDatabaseRO();
      fg::ItemId fromId {1, 1, 1, 4};
      fg::ItemId toId {1, 1, 1, 7};
      module.sqlSelectWords(fromId, toId);
      module.sqlCloseDatabase();

      fg::WordTextId id = (1L << 48) + (1L << 32) + (1L << 16) + 4L;
      std::stringstream ss;
      for(const auto& word : module.getWords()) {
        EXPECT_EQ(word.getId().getWordTextId(), id);
        EXPECT_EQ(word.getUuid(), id++);
        ss << word.getText() << " ";
      }
      EXPECT_STREQ(ss.str().c_str(), "God created the heaven ");
    }

    EXPECT_TRUE(true);
  } catch(const fg::filesystem::filesystem_error& e) {
    EXPECT_TRUE(false);
  } catch(const sqlite::sqlite_exception& e) {
    std::cout << "SQLITE ERROR CODE: " << e.get_code() << "\n"
              << e.what() << " during " << e.get_sql() << "\n";
    EXPECT_TRUE(false);
  } catch(...) {
    EXPECT_TRUE(false);
  }
}