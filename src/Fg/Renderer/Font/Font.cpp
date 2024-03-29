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

#include "Fg/Renderer/Font/Font.h"

#include <cassert>
#include <cstdio>
#include <limits>
#include <stdexcept>

namespace fg
{
namespace font
{
// default
Font::Font(Font&&) = default;
Font& Font::operator=(Font&&) = default;
Font::~Font() = default;

Font::Font(FtLibraryPtr ftLibrary, const int textCacheSize)
    : mFtLibrary {ftLibrary}
    , mFtFace {nullptr}
    , mHbBuffer {hb_buffer_create(), hb_buffer_destroy}
    , mHbFont {nullptr}
    , mCairoScaledFont {nullptr}
    , mScaledFontExtents {0.0, 0.0, 0.0, 0.0, 0.0}
    , mTextLayoutCache {std::make_shared<TextLayoutCache>(textCacheSize)}
    , mPixelSize {10}
    , mStrikeout {false}
    , mUnderline {false}
    , mxHeight {0.0}
{
}

bool Font::createFtFace(const Poco::File& fontFilePath, const int pixelSize)
{
  // NOTE: px = pt * DPI / 72
  mPixelSize = pixelSize;

  // FIXME: fileName.string() can be used on Windows only for ASCII code page.
  // TODO: see: https://stackoverflow.com/questions/10075032/can-freetype-functions-accept-unicode-filenames
  FT_Face ftFace;
  if(FT_New_Face(mFtLibrary.get(), fontFilePath.path().c_str(), 0, &ftFace)
     != FT_Err_Ok) {
    throw std::logic_error("FT_New_Face() != FT_Err_Ok");
  }
  mFtFace = {ftFace, FT_Done_Face};

  //  if(FT_Set_Pixel_Sizes(mFtFace.get(), 0, mPixelSize) != FT_Err_Ok) {
  //    throw std::logic_error("FT_Set_Pixel_Sizes() != FT_Err_Ok");
  //  }

  // We ignore encoding.
  if(forceUcs2Charmap(mFtFace.get()) != FT_Err_Ok) {
    throw std::logic_error("forceUcs2Charmap() != FT_Err_Ok");
  }

  mHbFont = {hb_ft_font_create(mFtFace.get(), nullptr), hb_font_destroy};
  if(!mHbFont) {
    throw std::logic_error("hb_ft_font_create() returns nullptr");
  }

  mCairoScaledFont =
      Cairo::getScaledFont(mFtFace.get(), FT_LOAD_DEFAULT, mPixelSize);

  //    mFtRasterParams.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
  return true;
}

// See http://www.microsoft.com/typography/otspec/name.htm
//    for a list of some possible platform-encoding pairs.
//    We're interested in 0-3 aka 3-1 - UCS-2.
//    Otherwise, fail. If a font has some unicode map, but lacks
//    UCS-2 - it is a broken or irrelevant font. What exactly
//    Freetype will select on face load (it promises most wide
//    unicode, and if that will be slower that UCS-2 - left as
//    an excercise to check.
// static
int Font::forceUcs2Charmap(FT_Face ftf)
{
  for(int i = 0; i < ftf->num_charmaps; i++) {
    auto& charmap = ftf->charmaps[i];
    if(((charmap->platform_id == 0) && (charmap->encoding_id == 3))
       || ((charmap->platform_id == 3) && (charmap->encoding_id == 1))) {
      return (FT_Set_Charmap(ftf, charmap));
    }
  }
  return FT_Err_Invalid_Argument;
}

void Font::resetBuffer()
{
  hb_buffer_reset(mHbBuffer.get());
}

void Font::clearBuffer()
{
  // Clean up the buffer, but don't kill it just yet.
  hb_buffer_clear_contents(mHbBuffer.get());
}

void Font::setDirection(const hb_direction_t direction)
{
  // NOTE: see also hb_script_get_horizontal_direction()
  hb_buffer_set_direction(mHbBuffer.get(), direction);
}

void Font::setScript(const hb_script_t script)
{
  hb_buffer_set_script(mHbBuffer.get(), script);  // see hb-unicode.h
}

void Font::setLanguage(const std::string& language)
{
  // For ISO 639 Code see
  // http://www.loc.gov/standards/iso639-2/php/code_list.php
  hb_buffer_set_language(
      mHbBuffer.get(),
      hb_language_from_string(
          language.c_str(), static_cast<int>(language.size())));
}

typename Font::TextLayoutPtr Font::getTextLayout(const std::string& text)
{
  // TODO: check buffer state.

  if(mTextLayoutCache->check(text)) {
    return mTextLayoutCache->fetch(text);
  }

  clearBuffer();

  // TODO: set Direction, Script and Language through Container's methods.
  setDirection(HB_DIRECTION_LTR);
  setScript(HB_SCRIPT_LATIN);
  setLanguage("eng");

  // Layout the text
  hb_buffer_add_utf8(
      mHbBuffer.get(), text.c_str(), static_cast<int>(text.size()), 0,
      static_cast<int>(text.size()));
  hb_shape(mHbFont.get(), mHbBuffer.get(), nullptr, 0);

  unsigned int glyphCount;
  hb_glyph_info_t* glyphInfo =
      hb_buffer_get_glyph_infos(mHbBuffer.get(), &glyphCount);
  hb_glyph_position_t* glyphPos =
      hb_buffer_get_glyph_positions(mHbBuffer.get(), &glyphCount);

  Cairo::GlyphVectorPtr glyphs =
      std::make_shared<Cairo::GlyphVector>(glyphCount);
  double x = 0;
  double y = 0;
  for(unsigned int i = 0; i < glyphCount; ++i) {
    (*glyphs)[i].index = glyphInfo[i].codepoint;
    (*glyphs)[i].x =
        x + (static_cast<double>(glyphPos[i].x_offset) / FT_64_DOUBLE);
    (*glyphs)[i].y =
        y - (static_cast<double>(glyphPos[i].y_offset) / FT_64_DOUBLE);
    x += static_cast<double>(glyphPos[i].x_advance) / FT_64_DOUBLE;
    y -= static_cast<double>(glyphPos[i].y_advance) / FT_64_DOUBLE;
  }

  Cairo::TextExtentsPtr textExtents = std::make_shared<cairo_text_extents_t>();
  cairo_scaled_font_glyph_extents(
      mCairoScaledFont.get(), glyphs->data(), static_cast<int>(glyphCount),
      textExtents.get());

  TextLayoutPtr textLayout = std::make_shared<TextLayout>(glyphs, textExtents);
  mTextLayoutCache->insert(text, textLayout);
  return textLayout;
}

const cairo_font_extents_t& Font::getScaledFontExtents()
{
  if(mScaledFontExtents.height > 0) {
    return mScaledFontExtents;
  }
  cairo_scaled_font_extents(mCairoScaledFont.get(), &mScaledFontExtents);
  return mScaledFontExtents;
}

Cairo::TextExtentsPtr Font::getTextExtents(const std::string& text)
{
  return getTextLayout(text)->mExtents;
}

void Font::drawText(
    CairoPtr cairo,
    const std::string& text,
    const double x,
    const double y,
    const Color& color)
{
  TextLayoutPtr textLayout = getTextLayout(text);
  cairo->showGlyphs(
      *textLayout->mGlyphs, mCairoScaledFont, x, y, *textLayout->mExtents,
      color);
}

double Font::xHeight()
{
  if(mxHeight > 0) {
    return mxHeight;
  }
  mxHeight = Cairo::xHeight(mCairoScaledFont);
  return mxHeight;
}

Font::TextLayout::TextLayout(
    Cairo::GlyphVectorPtr glyphs, Cairo::TextExtentsPtr textExtents) noexcept
    : mGlyphs {glyphs}
    , mExtents {textExtents}
{
}

}  // namespace font
}  // namespace fg
