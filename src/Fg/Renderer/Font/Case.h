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

#ifndef FG_FONT_CASE_H
#define FG_FONT_CASE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <fontconfig/fontconfig.h>

#include <ft2build.h>
#include FT_FREETYPE_H

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"

#pragma warning(push)
#pragma warning(disable : 4251)

#include "Poco/Path.h"

#pragma warning(pop)
#pragma warning(pop)

#include "Fg/Renderer/Font/Types.h"

namespace fg
{
namespace font
{
class Font;

class Case
{
public:
  enum class FontMatches : uint8_t
  {
    allMatched = 0,
    notMatchedFaceName = 1 << 0,
    notMatchedFontStyle = 1 << 1,
    notMatchedPixelSize = 1 << 2,
    notMatchedWeight = 1 << 3,
  };

  enum class FontStyle
  {
    fontStyleNormal = 0,
    fontStyleItalic,
  };

  // delete
  Case(const Case& other) = delete;
  Case& operator=(const Case& other) = delete;

  // default
  Case(Case&& other);
  ~Case();
  Case& operator=(Case&& other);

  explicit Case();

  bool parseAndLoadConfigFromMemory(
      const std::string& fontConfig, const bool complain);
  bool addFontDir(const Poco::File& dirPath);

  Poco::File getFontFilePath(
      const std::vector<std::string>& fontNames,
      const int pixelSize,
      const int weight,
      const FontStyle fontStyle,
      FontMatches* result) const;

  Font createFont(const int textCacheSize = 1000);
  Font* createNewFont(const int textCacheSize = 1000);
  void deleteFont(Font* font);

private:
  int weightToFcWeight(const int weigh) const;
  int fontStyleToFcSlant(const FontStyle fontStyle) const;

private:
  using FcConfigPtr = std::shared_ptr<FcConfig>;
  using FcPatternPtr = std::shared_ptr<FcPattern>;

  FcConfigPtr mFcConfig;
  FtLibraryPtr mFtLibrary;
};  // class Case

inline Case::FontMatches operator|=(
    Case::FontMatches a, const Case::FontMatches b)
{
  uint8_t c = static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
  return static_cast<Case::FontMatches>(c);
}

}  // namespace font
}  // namespace fg

#endif  // FG_FONT_CASE_H
