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

#include "Fg/Renderer/Font/Case.h"

#include <vector>

#include "Fg/Renderer/Font/Font.h"

namespace fg
{
namespace font
{
// default
Case::Case(Case&& other) = default;
Case::~Case() = default;
Case& Case::operator=(Case&& other) = default;

Case::Case()
    : mFcConfig {FcInitLoadConfig(), FcConfigDestroy}
    , mFtLibrary {nullptr}
{
  FT_Library frLibrary;
  FT_Init_FreeType(&frLibrary);
  mFtLibrary = {frLibrary, FT_Done_FreeType};
}

bool Case::parseAndLoadConfigFromMemory(
    const std::string& fontConfig, const bool complain)
{
  const FcChar8* config = reinterpret_cast<const FcChar8*>(fontConfig.c_str());
  return FcConfigParseAndLoadFromMemory(mFcConfig.get(), config, complain)
      && FcConfigSetCurrent(mFcConfig.get());
}

bool Case::addFontDir(const Poco::File& dirPath)
{
  // FIXME: std::fileName.string() can be used on Windows only for ASCII code page.
  const FcChar8* dir = reinterpret_cast<const FcChar8*>(dirPath.path().c_str());
  return FcConfigAppFontAddDir(mFcConfig.get(), dir);
}

Poco::File Case::getFontFilePath(
    const std::vector<std::string>& fontNames,
    const int pixelSize,
    const int weight,
    const FontStyle fontStyle,
    FontMatches* result) const
{
  Poco::File ret;

  FcPatternPtr pat {FcPatternCreate(), FcPatternDestroy};

  for(const auto& font : fontNames) {
    const FcChar8* fcFamily = reinterpret_cast<const FcChar8*>(font.c_str());
    FcPatternAddString(pat.get(), FC_FAMILY, fcFamily);
  }

  int fcSlant = fontStyleToFcSlant(fontStyle);
  int fcWeight = weightToFcWeight(weight);

  FcPatternAddInteger(pat.get(), FC_SLANT, fcSlant);
  FcPatternAddInteger(pat.get(), FC_PIXEL_SIZE, pixelSize);
  FcPatternAddInteger(pat.get(), FC_WEIGHT, fcWeight);

  FcConfigSubstitute(mFcConfig.get(), pat.get(), FcMatchPattern);
  FcDefaultSubstitute(pat.get());

  // Find the font.
  FcResult fcResult;
  FcPatternPtr fontPat {
      FcFontMatch(mFcConfig.get(), pat.get(), &fcResult), FcPatternDestroy};

  if(fontPat && FcResultMatch == fcResult) {
    FcChar8* file = nullptr;
    if(FcPatternGetString(fontPat.get(), FC_FILE, 0, &file) == FcResultMatch) {
      // Found the font file, this might be a fallback font.
      ret = reinterpret_cast<char*>(file);

      if(result) {
        FcChar8* retFamily = nullptr;
        int retSlant = -1;
        int retPixelSize = -1;
        int retWeight = -1;
        *result = FontMatches::allMatched;

        if(FcPatternGetString(fontPat.get(), FC_FAMILY, 0, &retFamily)
           == FcResultMatch) {
          bool found = false;
          for(const auto& font : fontNames) {
            if(0 == font.compare(reinterpret_cast<const char*>(retFamily))) {
              found = true;
              break;
            }
          }
          if(!found) {
            *result |= FontMatches::notMatchedFaceName;
          }
        }

        if(FcPatternGetInteger(fontPat.get(), FC_SLANT, 0, &retSlant)
               == FcResultMatch
           && retSlant != fcSlant) {
          *result |= FontMatches::notMatchedFontStyle;
        }
        if(FcPatternGetInteger(fontPat.get(), FC_PIXEL_SIZE, 0, &retPixelSize)
               == FcResultMatch
           && retPixelSize != pixelSize) {
          *result |= FontMatches::notMatchedPixelSize;
        }
        if(FcPatternGetInteger(fontPat.get(), FC_WEIGHT, 0, &retWeight)
               == FcResultMatch
           && retWeight != fcWeight) {
          *result |= FontMatches::notMatchedWeight;
        }
      }
    }
  }

  return ret;
}

Font Case::createFont(const int textCacheSize /*= 1000*/)
{
  return fg::font::Font {mFtLibrary, textCacheSize};
}

Font* Case::createNewFont(const int textCacheSize /*= 1000*/)
{
  return new fg::font::Font {mFtLibrary, textCacheSize};
}

void Case::deleteFont(Font* font)
{
  if(font) {
    delete font;
  }
}

inline int Case::weightToFcWeight(const int weight) const
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

inline int Case::fontStyleToFcSlant(const FontStyle fontStyle) const
{
  switch(fontStyle) {
    case FontStyle::fontStyleItalic:
      return FC_SLANT_ITALIC;
    case FontStyle::fontStyleNormal:
    default:
      return FC_SLANT_ROMAN;
  }
}

}  // namespace font
}  // namespace fg
