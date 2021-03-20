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

#ifndef FG_FONT_LIBRARY_H
#define FG_FONT_LIBRARY_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <fontconfig/fontconfig.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Poco/File.h"
#include "Poco/Path.h"

#include "Fg/Shared/Types.h"

namespace fg
{
class FontLibrary;

using FontLibraryPtr = std::shared_ptr<FontLibrary>;

using FcConfigPtr = std::shared_ptr<FcConfig>;
using FcPatternPtr = std::shared_ptr<FcPattern>;
using FtLibraryPtr = std::shared_ptr<FT_LibraryRec_>;

class FontLibrary
{
public:
  enum FontMatches : uint_least8_t
  {
    allMatched = 0,
    notMatchedFaceName = 1 << 0,
    notMatchedFontStyle = 1 << 1,
    notMatchedPixelSize = 1 << 2,
    notMatchedWeight = 1 << 3,
  };

  // TODO: Copy/move constructors/operators.
  explicit FontLibrary();
  ~FontLibrary();

  bool parseAndLoadConfigFromMemory(
      const std::string& fontConfig, const bool complain);
  bool addFontDir(const Poco::File& dirPath);

  Poco::File getFontFilePath(
      const std::vector<String>& fontNames,
      const int pixelSize,
      const int weight,
      const FontStyle fontStyle,
      uint_least8_t* result) const;

  FtLibraryPtr ftLibrary() { return mFtLibrary; }

private:
  int weightToFcWeight(const int weigh) const;
  int fontStyleToFcSlant(const FontStyle fontStyle) const;

private:
  FcConfigPtr mFcConfig;
  FtLibraryPtr mFtLibrary;
};  // class FontLibrary

inline int FontLibrary::weightToFcWeight(const int weight) const
{
  if(weight >= 0 && weight < 150)
    return FC_WEIGHT_THIN;
  else if(weight >= 150 && weight < 250)
    return FC_WEIGHT_EXTRALIGHT;
  else if(weight >= 250 && weight < 350)
    return FC_WEIGHT_LIGHT;
  else if(weight >= 350 && weight < 450)
    return FC_WEIGHT_NORMAL;
  else if(weight >= 450 && weight < 550)
    return FC_WEIGHT_MEDIUM;
  else if(weight >= 550 && weight < 650)
    return FC_WEIGHT_SEMIBOLD;
  else if(weight >= 650 && weight < 750)
    return FC_WEIGHT_BOLD;
  else if(weight >= 750 && weight < 850)
    return FC_WEIGHT_EXTRABOLD;
  else if(weight >= 950)
    return FC_WEIGHT_BLACK;
  else
    return FC_WEIGHT_NORMAL;
}

inline int FontLibrary::fontStyleToFcSlant(const FontStyle fontStyle) const
{
  switch(fontStyle) {
    case FontStyle::fontStyleItalic:
      return FC_SLANT_ITALIC;
    case FontStyle::fontStyleNormal:
    default:
      return FC_SLANT_ROMAN;
  }
}

}  // namespace fg

#endif  // FG_FONT_LIBRARY_H
