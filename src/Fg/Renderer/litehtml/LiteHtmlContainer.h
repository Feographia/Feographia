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

#ifndef FG_LITE_HTML_CONTAINER_H
#define FG_LITE_HTML_CONTAINER_H

#include <memory>
#include <string>
#include <utility>

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 4458 4100)
#pragma warning(disable : 4365 4365)  // off by default

#include "litehtml/litehtml.h"

#pragma warning(pop)

// MSVC pragmas
#pragma warning(push)
#pragma warning(disable : 5039 4619)

#include "Poco/File.h"

#pragma warning(pop)

#include "Fg/Renderer/Font/Case.h"
#include "Fg/Shared/Types.h"

namespace fg
{
class LiteHtmlContainer;

using LiteHtmlContainerPtr = std::shared_ptr<LiteHtmlContainer>;

class LiteHtmlContainer : public litehtml::document_container
{
public:
  // TODO: Copy/move constructors/operators.
  explicit LiteHtmlContainer();
  virtual ~LiteHtmlContainer() override;

  bool parseAndLoadFontConfigFromMemory(
      const std::string& fontConfig, bool complain);
  bool addFontDir(const Poco::File& dirPath);
  void setDefaultFontName(const std::string& name);
  void setDefaultFontSize(int size);
  void setFontTextCacheSize(int size);

  void setDeviceWidth(int width);
  void setDeviceHeight(int height);
  void setDeviceDpiX(int dpi);
  void setDeviceDpiY(int dpi);
  void setDisplayAreaWidth(int width);
  void setDisplayAreaHeight(int height);

  void setDeviceMonochromeBits(int bits);
  void setDeviceColorBits(int bits);
  void setDeviceColorIndex(int colorIndex);
  void setDeviceMediaType(litehtml::media_type type);

  static Color getColorFromWebColor(litehtml::web_color webColor);

  // litehtml::document_container interface.
  virtual litehtml::uint_ptr create_font(
      const litehtml::tchar_t* faceName,
      int size,
      int weight,
      litehtml::font_style italic,
      unsigned int decoration,
      litehtml::font_metrics* fm) override;
  virtual void delete_font(litehtml::uint_ptr hFont) override;
  virtual int text_width(
      const litehtml::tchar_t* text, litehtml::uint_ptr hFont) override;
  virtual void draw_text(
      litehtml::uint_ptr hdc,
      const litehtml::tchar_t* text,
      litehtml::uint_ptr hFont,
      litehtml::web_color color,
      const litehtml::position& pos) override;
  virtual int pt_to_px(int pt) override;
  virtual int get_default_font_size() const override;
  virtual const litehtml::tchar_t* get_default_font_name() const override;
  virtual void draw_list_marker(
      litehtml::uint_ptr hdc, const litehtml::list_marker& marker) override;
  virtual void load_image(
      const litehtml::tchar_t* src,
      const litehtml::tchar_t* baseurl,
      bool redraw_on_ready) override;
  virtual void get_image_size(
      const litehtml::tchar_t* src,
      const litehtml::tchar_t* baseurl,
      litehtml::size& sz) override;
  virtual void draw_background(
      litehtml::uint_ptr hdc, const litehtml::background_paint& bg) override;
  virtual void draw_borders(
      litehtml::uint_ptr hdc,
      const litehtml::borders& borders,
      const litehtml::position& draw_pos,
      bool root) override;

  virtual void set_caption(const litehtml::tchar_t* caption) override;
  virtual void set_base_url(const litehtml::tchar_t* base_url) override;
  virtual void link(
      const std::shared_ptr<litehtml::document>& ptr,
      const litehtml::element::ptr& el) override;
  virtual void on_anchor_click(
      const litehtml::tchar_t* url, const litehtml::element::ptr& el) override;
  virtual void set_cursor(const litehtml::tchar_t* cursor) override;
  virtual void transform_text(
      litehtml::tstring& text, litehtml::text_transform tt) override;
  virtual void import_css(
      litehtml::tstring& text,
      const litehtml::tstring& url,
      litehtml::tstring& baseurl) override;
  virtual void set_clip(
      const litehtml::position& pos,
      const litehtml::border_radiuses& bdr_radius,
      bool valid_x,
      bool valid_y) override;
  virtual void del_clip() override;
  virtual void get_client_rect(litehtml::position& client) const override;
  virtual std::shared_ptr<litehtml::element> create_element(
      const litehtml::tchar_t* tag_name,
      const litehtml::string_map& attributes,
      const std::shared_ptr<litehtml::document>& doc) override;

  virtual void get_media_features(
      litehtml::media_features& media) const override;
  virtual void get_language(
      litehtml::tstring& language, litehtml::tstring& culture) const override;
  //virtual litehtml::tstring resolve_color(
  //    const litehtml::tstring& color) const override;

private:
  font::CasePtr mFontCase;

  std::string mFontDefaultName;
  int mDefaultFontSize;
  int mFontTextCacheSize;

  // (pixels) The width of the rendering surface of the output device.
  // For continuous media, this is the width of the screen.
  // For paged media, this is the width of the page sheet size.
  int mDeviceWidth;

  // (pixels) The height of the rendering surface of the output device.
  // For continuous media, this is the height of the screen.
  // For paged media, this is the height of the page sheet size.
  int mDeviceHeight;

  // The resolution of the output device of the axis X (in DPI).
  int mDeviceDpiX;

  // The resolution of the output device of the axis Y (in DPI).
  int mDeviceDpiY;

  // (pixels) The width of the targeted display area of the output device.
  // For continuous media, this is the width of the viewport
  // including the size of a rendered scroll bar (if any).
  // For paged media, this is the width of the page box.
  int mDisplayAreaWidth;

  // (pixels) The height of the targeted display area of the output device.
  // For continuous media, this is the height of the viewport
  // including the size of a rendered scroll bar (if any).
  // For paged media, this is the height of the page box.
  int mDisplayAreaHeight;

  // The number of bits per pixel in a monochrome frame buffer.
  // If the device is not a monochrome device,
  // the output device value will be 0.
  int mDeviceMonochromeBits;

  // The number of bits per color component of the output device.
  // If the device is not a color device, the value is zero.
  int mDeviceColorBits;

  // The number of entries in the color lookup table of the output device.
  // If the device does not use a color lookup table, the value is zero.
  int mDeviceColorIndex;

  litehtml::media_type mDeviceMediaType;
};  // class Container

inline void LiteHtmlContainer::setDefaultFontName(const std::string& name)
{
  mFontDefaultName = name;
}

inline void LiteHtmlContainer::setDefaultFontSize(int size)
{
  mDefaultFontSize = size;
}

inline void LiteHtmlContainer::setFontTextCacheSize(int size)
{
  mFontTextCacheSize = size;
}

inline void LiteHtmlContainer::setDeviceWidth(int width)
{
  mDeviceWidth = width;
}

inline void LiteHtmlContainer::setDeviceHeight(int height)
{
  mDeviceHeight = height;
}

inline void LiteHtmlContainer::setDeviceDpiX(int dpi)
{
  mDeviceDpiX = dpi;
}

inline void LiteHtmlContainer::setDeviceDpiY(int dpi)
{
  mDeviceDpiY = dpi;
}

inline void LiteHtmlContainer::setDisplayAreaWidth(int width)
{
  mDisplayAreaWidth = width;
}

inline void LiteHtmlContainer::setDisplayAreaHeight(int height)
{
  mDisplayAreaHeight = height;
}

inline void LiteHtmlContainer::setDeviceMonochromeBits(int bits)
{
  mDeviceMonochromeBits = bits;
}

inline void LiteHtmlContainer::setDeviceColorBits(int bits)
{
  mDeviceColorBits = bits;
}

inline void LiteHtmlContainer::setDeviceColorIndex(int colorIndex)
{
  mDeviceColorIndex = colorIndex;
}

inline void LiteHtmlContainer::setDeviceMediaType(litehtml::media_type type)
{
  mDeviceMediaType = type;
}

// static
inline Color LiteHtmlContainer::getColorFromWebColor(
    litehtml::web_color webColor)
{
  return Color {
      webColor.red / 255.0, webColor.green / 255.0, webColor.blue / 255.0,
      webColor.alpha / 255.0};
}

}  // namespace fg

#endif  // FG_LITE_HTML_CONTAINER_H
