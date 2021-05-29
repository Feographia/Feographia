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

#ifndef FG_TEXT_H
#define FG_TEXT_H

#include <cstdint>
#include <deque>
#include <utility>
#include <vector>

#include "unicode/unistr.h"

// GCC pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wnoexcept"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wswitch-default"

// Clang pragmas
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wshadow-field-in-constructor"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wweak-vtables"

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 4065 4244)
#pragma warning(disable : 4623 4464 4365 4365)  // off by default

#include <sqlite_modern_cpp.h>

#pragma warning(pop)
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"

#pragma warning(pop)

#include "Fg/Shared/Types.h"

namespace fg
{
using WordId = std::int_least16_t;
using WordTextId = std::int_least64_t;
using ItemUuid = WordTextId;

static constexpr WordId NULL_ID = 0;

class ItemId
{
public:
  ItemId() = delete;
  ItemId& operator=(const ItemId& other) = delete;
  ItemId& operator=(ItemId&& other) = delete;

  ItemId(const ItemId& other) = default;
  ItemId(ItemId&& other) = default;
  ~ItemId() = default;

  explicit ItemId(
      const WordId& bookId,
      const WordId& chapterId,
      const WordId& verseId,
      const WordId& wordId);

  explicit ItemId(
      WordId&& bookId, WordId&& chapterId, WordId&& verseId, WordId&& wordId);

  const WordId& getBookId() const { return mBookId; }
  void setBookId(const WordId& bookId) { mBookId = bookId; }

  const WordId& getChapterId() const { return mChapterId; }
  void setChapterId(const WordId& chapterId) { mChapterId = chapterId; }

  const WordId& getVerseId() const { return mVerseId; }
  void setVerseId(const WordId& verseId) { mVerseId = verseId; }

  const WordId& getWordId() const { return mWordId; }
  void setWordId(const WordId& wordId) { mWordId = wordId; }

  WordTextId getWordTextId() const;

private:
  WordId mBookId;
  WordId mChapterId;
  WordId mVerseId;
  WordId mWordId;
};

inline ItemId::ItemId(
    const WordId& bookId,
    const WordId& chapterId,
    const WordId& verseId,
    const WordId& wordId)
    : mBookId {bookId}
    , mChapterId {chapterId}
    , mVerseId {verseId}
    , mWordId {wordId}
{
}

inline ItemId::ItemId(
    WordId&& bookId, WordId&& chapterId, WordId&& verseId, WordId&& wordId)
    : mBookId {std::move(bookId)}
    , mChapterId {std::move(chapterId)}
    , mVerseId {std::move(verseId)}
    , mWordId {std::move(wordId)}
{
}

inline WordTextId ItemId::getWordTextId() const
{
  WordTextId id = mBookId;
  id <<= 16;
  id += mChapterId;
  id <<= 16;
  id += mVerseId;
  id <<= 16;
  id += mWordId;
  return id;
}


class Word
{
public:
  Word() = delete;
  Word& operator=(const Word& other) = delete;
  Word& operator=(Word&& other) = delete;

  Word(const Word& other) = default;
  Word(Word&& other) = default;
  ~Word() = default;

  explicit Word(
      const ItemId& id,
      const icu::UnicodeString& text,
      const ItemUuid& uuid = {});
  explicit Word(ItemId&& id, icu::UnicodeString&& text, ItemUuid&& uuid = {});

  explicit Word(
      const ItemId& id, const std::string& utf8Text, const ItemUuid& uuid = {});
  explicit Word(ItemId&& id, std::string&& utf8Text, ItemUuid&& uuid = {});

  const ItemId& getId() const { return mId; }
  const ItemUuid& getUuid() const { return mUuid; }

  const icu::UnicodeString& getText() const { return mText; }
  void setText(const icu::UnicodeString& text) { mText = text; }
  void setText(icu::UnicodeString&& text) { mText = std::move(text); }

  std::string getUtf8Text() const;
  void setText(const std::string& utf8Text);
  void setText(std::string&& utf8Text);

private:
  ItemId mId;
  icu::UnicodeString mText;
  const ItemUuid mUuid;

  // IF uuid != NULL THEN uuid ELSE id.
  // Use IFNULL() in SQLite.
  // See: https://www.sqlitetutorial.net/sqlite-functions/sqlite-ifnull/
};

inline Word::Word(
    const ItemId& id, const icu::UnicodeString& text, const ItemUuid& uuid)
    : mId {id}
    , mText {text}
    , mUuid {uuid}
{
}

inline Word::Word(ItemId&& id, icu::UnicodeString&& text, ItemUuid&& uuid)
    : mId {std::move(id)}
    , mText {std::move(text)}
    , mUuid {std::move(uuid)}
{
}

inline Word::Word(
    const ItemId& id, const std::string& utf8Text, const ItemUuid& uuid)
    : mId {id}
    , mText {icu::UnicodeString::fromUTF8(utf8Text)}
    , mUuid {uuid}
{
}

inline Word::Word(ItemId&& id, std::string&& utf8Text, ItemUuid&& uuid)
    : mId {std::move(id)}
    , mText {icu::UnicodeString::fromUTF8(std::move(utf8Text))}
    , mUuid {std::move(uuid)}
{
}

inline std::string Word::getUtf8Text() const
{
  std::string result;
  return mText.toUTF8String(result);
}

inline void Word::setText(const std::string& utf8Text)
{
  mText = icu::UnicodeString::fromUTF8(utf8Text);
}

inline void Word::setText(std::string&& utf8Text)
{
  mText = icu::UnicodeString::fromUTF8(std::move(utf8Text));
}


class Text
{
public:
  Text() = delete;
  Text& operator=(const Text& other) = delete;
  Text& operator=(Text&& other) = delete;

  Text(const Text& other) = default;
  Text(Text&& other) = default;

  Text(Poco::File moduleFile);
  ~Text();

  void sqlCreateDatabase();
  void sqlOpenDatabaseRW();
  void sqlOpenDatabaseRO();
  void sqlCloseDatabase();

  void sqlCreateTable();
  void sqlInsertWords(const std::vector<Word> words);
  void sqlSelectWords(const ItemId& fromId, const ItemId& toId);

  const std::deque<Word>& getWords() { return mWords; }

protected:
  void sqlOpenDatabase(sqlite::OpenFlags flags);

private:
  Poco::File mModuleFile;
  sqlite::connection_type mDbConnection;

  std::deque<Word> mWords;
};

inline Text::Text(Poco::File moduleFile)
    : mModuleFile {std::move(moduleFile)}
    , mDbConnection {nullptr}
    , mWords {}
{
}

}  // namespace fg

#endif  // FG_TEXT_H
