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

#include <iostream>
#include <sstream>
#include <utility>

#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Path.h"

#include "gtest/gtest.h"

#include "Fg/Module/Bible/Text/Text.h"

extern Poco::Path testDir;

TEST(TextTest, sqlCreateModule)
{
  try {
    EXPECT_TRUE(Poco::File {testDir}.exists());

    Poco::File moduleFile {
        Poco::Path {testDir}.setFileName("testModule.SQLite3")};
    if(moduleFile.exists()) {
      moduleFile.remove();
    }
    EXPECT_FALSE(moduleFile.exists());

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

      fg::WordTextId id = (1LL << 48) + (1LL << 32) + (1LL << 16) + 1LL;
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

      fg::WordTextId id = (1LL << 48) + (1LL << 32) + (1LL << 16) + 4LL;
      std::stringstream ss;
      for(const auto& word : module.getWords()) {
        EXPECT_EQ(word.getId().getWordTextId(), id);
        EXPECT_EQ(word.getUuid(), id++);
        ss << word.getText() << " ";
      }
      EXPECT_STREQ(ss.str().c_str(), "God created the heaven ");
    }

    EXPECT_TRUE(true);
  } catch(const Poco::FileException& /*e*/) {
    EXPECT_TRUE(false);
  } catch(const sqlite::sqlite_exception& e) {
    std::cout << "SQLITE ERROR CODE: " << e.get_code() << "\n"
              << e.what() << " during " << e.get_sql() << "\n";
    EXPECT_TRUE(false);
  } catch(...) {
    EXPECT_TRUE(false);
  }
}
