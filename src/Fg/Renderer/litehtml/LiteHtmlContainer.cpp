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

#include "Fg/Renderer/litehtml/LiteHtmlContainer.h"

#include "Fg/Renderer/Cairo.h"
#include "Fg/Renderer/Font/Case.h"
#include "Fg/Renderer/Font/Font.h"

namespace fg
{
LiteHtmlContainer::LiteHtmlContainer()
    : mFontCase {std::make_shared<font::Case>()}
    , mFontDefaultName {"Times New Roman"}
    , mDefaultFontSize {16}
    , mFontTextCacheSize {1000}
    , mDeviceWidth {320}
    , mDeviceHeight {240}
    , mDeviceDpiX {96}
    , mDeviceDpiY {96}
    , mDisplayAreaWidth {320}
    , mDisplayAreaHeight {240}
    , mDeviceMonochromeBits {0}
    , mDeviceColorBits {8}
    , mDeviceColorIndex {256}
    , mDeviceMediaType {litehtml::media_type_screen}
{
}

LiteHtmlContainer::~LiteHtmlContainer() = default;

bool LiteHtmlContainer::parseAndLoadFontConfigFromMemory(
    const std::string& fontConfig, bool complain)
{
  return mFontCase->parseAndLoadConfigFromMemory(fontConfig, complain);
}

bool LiteHtmlContainer::addFontDir(const Poco::File& dirPath)
{
  return mFontCase->addFontDir(dirPath);
}

litehtml::uint_ptr LiteHtmlContainer::create_font(
    const litehtml::tchar_t* faceName,
    int size,
    int weight,
    litehtml::font_style italic,
    unsigned int decoration,
    litehtml::font_metrics* fm)
{
  if(!fm) {
    return reinterpret_cast<litehtml::uint_ptr>(nullptr);
  }

  litehtml::string_vector fonts;
  litehtml::split_string(faceName, fonts, ",");
  for(auto& font : fonts) {
    litehtml::trim(font);
  }

  font::Case::FontMatches result;
  // TODO: convert fonts to std::vector<String>.
  // TODO: convert italic to FontStyle.
  Poco::File filePath = mFontCase->getFontFilePath(
      fonts, size, weight, static_cast<font::Case::FontStyle>(italic), &result);
  if(filePath.path().empty() || font::Case::FontMatches::allMatched != result) {
    return reinterpret_cast<litehtml::uint_ptr>(nullptr);
  }

  font::Font* font = mFontCase->createNewFont(mFontTextCacheSize);
  if(!font->createFtFace(filePath, size)) {
    return reinterpret_cast<litehtml::uint_ptr>(nullptr);
  }
  font->setUnderline(decoration & litehtml::font_decoration_underline);
  font->setStrikeout(decoration & litehtml::font_decoration_linethrough);

  // Note: for font metric precision (in particular for TTF) see
  // https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Size_Metrics
  const cairo_font_extents_t& fontExtents = font->getScaledFontExtents();
  fm->ascent = static_cast<int>(fontExtents.ascent);
  fm->descent = static_cast<int>(-fontExtents.descent);
  fm->height = static_cast<int>(fontExtents.height);
  fm->x_height = static_cast<int>(font->xHeight());
  if(italic == litehtml::fontStyleItalic || decoration) {
    fm->draw_spaces = true;
  } else {
    fm->draw_spaces = false;
  }

  return reinterpret_cast<litehtml::uint_ptr>(font);
}

void LiteHtmlContainer::delete_font(litehtml::uint_ptr hFont)
{
  font::Font* font = reinterpret_cast<font::Font*>(hFont);
  mFontCase->deleteFont(font);
}

int LiteHtmlContainer::text_width(
    const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{
  font::Font* font = reinterpret_cast<font::Font*>(hFont);
  if(!font) {
    return 0;
  }

  Cairo::TextExtentsPtr extents = font->getTextExtents(text);
  return static_cast<int>(extents->x_advance - extents->x_bearing);
}

void LiteHtmlContainer::draw_text(
    litehtml::uint_ptr hdc,
    const litehtml::tchar_t* text,
    litehtml::uint_ptr hFont,
    litehtml::web_color color,
    const litehtml::position& pos)
{
  CairoPtr& cairo = *(reinterpret_cast<CairoPtr*>(hdc));
  if(!cairo) {
    return;
  }

  if(!text) {
    return;
  }

  font::Font* font = reinterpret_cast<font::Font*>(hFont);
  if(!font) {
    return;
  }

  const cairo_font_extents_t& fontExtents = font->getScaledFontExtents();
  int x = pos.left();
  int y = static_cast<int>(pos.bottom() - fontExtents.descent);

  font->drawText(
      cairo, text, x, y,
      Color {LiteHtmlContainer::getColorFromWebColor(color)});

  if(font->underline() || font->strikeout()) {
    int tw = text_width(text, hFont);

    if(font->underline()) {
      // TODO: set line width by font's height.
      // TODO: set line position by font's parameters.
      //cairo->drawLine(x, y + 1.5, x + tw, y + 1.5, 1,
      //    Color{Container::getColorFromWebColor(color)});
      cairo->drawLine(
          x, y + 3, x + tw, y + 3, 1.5,
          Color {LiteHtmlContainer::getColorFromWebColor(color)});
    }

    if(font->strikeout()) {
      // TODO: set line width by font's height.
      int lnY = static_cast<int>(y - font->xHeight() / 2.0);
      //cairo->drawLine(x, lnY - 0.5, x + tw, lnY - 0.5, 1,
      //    Color{Container::getColorFromWebColor(color)});
      cairo->drawLine(
          x, lnY, x + tw, lnY, 1.5,
          Color {LiteHtmlContainer::getColorFromWebColor(color)});
    }
  }
}

int LiteHtmlContainer::pt_to_px(int pt)
{
  return static_cast<int>(static_cast<double>(pt) * mDeviceDpiY / 72.0);
}

int LiteHtmlContainer::get_default_font_size() const
{
  return mDefaultFontSize;
}

const litehtml::tchar_t* LiteHtmlContainer::get_default_font_name() const
{
  return mFontDefaultName.c_str();
}

void LiteHtmlContainer::draw_list_marker(
    litehtml::uint_ptr /*hdc*/, const litehtml::list_marker& /*marker*/)
{
}

void LiteHtmlContainer::load_image(
    const litehtml::tchar_t* /*src*/,
    const litehtml::tchar_t* /*baseurl*/,
    bool /*redraw_on_ready*/)
{
}

void LiteHtmlContainer::get_image_size(
    const litehtml::tchar_t* /*src*/,
    const litehtml::tchar_t* /*baseurl*/,
    litehtml::size& /*sz*/)
{
}

void LiteHtmlContainer::draw_background(
    litehtml::uint_ptr /*hdc*/, const litehtml::background_paint& /*bg*/)
{
}

void LiteHtmlContainer::draw_borders(
    litehtml::uint_ptr /*hdc*/,
    const litehtml::borders& /*borders*/,
    const litehtml::position& /*draw_pos*/,
    bool /*root*/)
{
}

void LiteHtmlContainer::set_caption(const litehtml::tchar_t* /*caption*/) {}

void LiteHtmlContainer::set_base_url(const litehtml::tchar_t* /*base_url*/) {}

void LiteHtmlContainer::link(
    const std::shared_ptr<litehtml::document>& /*ptr*/,
    const litehtml::element::ptr& /*el*/)
{
}

void LiteHtmlContainer::on_anchor_click(
    const litehtml::tchar_t* /*url*/, const litehtml::element::ptr& /*el*/)
{
}

void LiteHtmlContainer::set_cursor(const litehtml::tchar_t* /*cursor*/) {}

void LiteHtmlContainer::transform_text(
    litehtml::tstring& /*text*/, litehtml::text_transform /*tt*/)
{
}

void LiteHtmlContainer::import_css(
    litehtml::tstring& /*text*/,
    const litehtml::tstring& /*url*/,
    litehtml::tstring& /*baseurl*/)
{
}

void LiteHtmlContainer::set_clip(
    const litehtml::position& /*pos*/,
    const litehtml::border_radiuses& /*bdr_radius*/,
    bool /*valid_x*/,
    bool /*valid_y*/)
{
}

void LiteHtmlContainer::del_clip() {}

void LiteHtmlContainer::get_client_rect(litehtml::position& client) const
{
  client.width = mDisplayAreaWidth;
  client.height = mDisplayAreaHeight;
}

std::shared_ptr<litehtml::element> LiteHtmlContainer::create_element(
    const litehtml::tchar_t* /*tag_name*/,
    const litehtml::string_map& /*attributes*/,
    const std::shared_ptr<litehtml::document>& /*doc*/)
{
  return nullptr;
}

void LiteHtmlContainer::get_media_features(
    litehtml::media_features& media) const
{
  litehtml::position clientRect;
  get_client_rect(clientRect);

  media.type = mDeviceMediaType;
  media.width = clientRect.width;
  media.height = clientRect.height;
  media.color = mDeviceColorBits;
  media.color_index = mDeviceColorIndex;
  media.monochrome = mDeviceMonochromeBits;
  media.resolution = mDeviceDpiX;
  media.device_width = mDeviceWidth;
  media.device_height = mDeviceHeight;
}

void LiteHtmlContainer::get_language(
    litehtml::tstring& language, litehtml::tstring& culture) const
{
  language = _t("en");
  culture = _t("");
}

//litehtml::tstring Container::resolve_color(
//    const litehtml::tstring& color) const
//{
//  return litehtml::tstring();
//}

}  // namespace fg
