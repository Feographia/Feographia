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

#ifndef FG_ITEM_H
#define FG_ITEM_H

#include <cstdint>
#include <utility>

#include "Fg/Shared/Types.h"

namespace fg
{
class Item
{
public:
  using Id = std::int_least32_t;  // Max 800'000 words in the Bible.

  static constexpr Id NULL_ID = 0;

  Item() = delete;
  Item& operator=(const Item& other) = delete;
  Item& operator=(Item&& other) = delete;

  Item(const Item& other) = default;
  Item(Item&& other) = default;
  ~Item() = default;

  explicit Item(const String& word, const Id id, const Id uuid = NULL_ID);
  explicit Item(String&& word, const Id id, const Id uuid = NULL_ID);

  const String& getWord() const { return mWord; }
  void setWord(const String& word) { mWord = word; }

  const Id& getId() const { return mId; }
  void setId(const Id& id) { mId = id; }

  const Id& getUuid() const { return mUuid; }

private:
  String mWord;
  String mFormattedWord;
  // IF uuid == NULL THEN uuid ELSE id.
  // Use IFNULL() in SQLite.
  // See: https://www.sqlitetutorial.net/sqlite-functions/sqlite-ifnull/
  Id mId;
  const Id mUuid;
};

inline Item::Item(
    const String& word, const Id id, const Id uuid /*= NULL_ID*/)
    : mWord{word}
    , mId{id}
    , mUuid{uuid == NULL_ID ? id : uuid}
{
}

inline Item::Item(String&& word, const Id id, const Id uuid /*= NULL_ID*/)
    : mWord{std::move(word)}
    , mId{id}
    , mUuid{uuid == NULL_ID ? id : uuid}
{
}

}  // namespace fg

#endif  // FG_ITEM_H
