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

#include "Fg/Module/Bible/Text/Text.h"

#include "Poco/Exception.h"

namespace fg
{
Text::~Text() = default;

void Text::sqlCreateDatabase()
{
  if(mModuleFile.exists()) {
    throw new Poco::FileExistsException(
        "Database file is existing", mModuleFile.path());
  }

  sqlOpenDatabase(sqlite::OpenFlags::READWRITE | sqlite::OpenFlags::CREATE);
}

void Text::sqlOpenDatabaseRW()
{
  sqlOpenDatabase(sqlite::OpenFlags::READWRITE);
}

void Text::sqlOpenDatabaseRO()
{
  sqlOpenDatabase(sqlite::OpenFlags::READONLY);
}

void Text::sqlCloseDatabase()
{
  if(mDbConnection) {
    mDbConnection = nullptr;
  }
}

void Text::sqlOpenDatabase(sqlite::OpenFlags flags)
{
  if(!mDbConnection) {
    sqlite::sqlite_config config;
    config.flags = flags;
    // The encoding is respected only if you create a new database.
    config.encoding = sqlite::Encoding::UTF8;
    sqlite::database db {mModuleFile.path(), config};

    mDbConnection = db.connection();
  }
}

void Text::sqlCreateTable()
{
  const char* query =
#include "Fg/Module/Bible/Text/TextCreateTable.sql"
      ;
  sqlite::database db {mDbConnection};
  db << query;
}

void Text::sqlInsertWords(const std::vector<Word> words)
{
  const char* query =
#include "Fg/Module/Bible/Text/TextInsertWords.sql"
      ;
  sqlite::database db {mDbConnection};
  auto stmt = db << query;

  for(const auto& word : words) {
    const auto& id = word.getId();
    stmt << id.getBookId() << id.getChapterId() << id.getVerseId()
         << id.getWordId() << word.getText();
    stmt++;
  }
}

void Text::sqlSelectWords(const ItemId& fromId, const ItemId& toId)
{
  const std::string query =
#include "Fg/Module/Bible/Text/TextSelectWords.sql"
      ;
  sqlite::database db {mDbConnection};
  auto stmt = db << query;

  stmt << fromId.getBookId() << fromId.getChapterId() << fromId.getVerseId()
       << fromId.getWordId();
  stmt << toId.getBookId() << toId.getChapterId() << toId.getVerseId()
       << toId.getWordId();

  stmt >> [&](WordId&& bookId, WordId&& chapterId, WordId&& verseId,
              WordId&& wordId, String&& text, ItemUuid&& uuid) {
    ItemId id {
        std::move(bookId), std::move(chapterId), std::move(verseId),
        std::move(wordId)};
    // TODO: 1) use direction and pop from deque if deque size is bigger.then value.
    // TODO: 2) check if words is existing in deque
    // TODO: 1) and 2) in seperated method.
    mWords.emplace_back(std::move(id), std::move(text), std::move(uuid));
  };
}

}  // namespace fg
