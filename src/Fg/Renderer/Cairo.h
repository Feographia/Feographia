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

#ifndef FG_CAIRO_H
#define FG_CAIRO_H


#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cairo/cairo.h>

#include "Fg/Shared/Types.h"

namespace fg
{
class Cairo;

using CairoPtr = std::shared_ptr<Cairo>;

class Cairo
{
public:
  using SurfacePtr = std::shared_ptr<cairo_surface_t>;
  using ContextPtr = std::shared_ptr<cairo_t>;
  using FontFacePtr = std::shared_ptr<cairo_font_face_t>;
  using ScaledFontPtr = std::shared_ptr<cairo_scaled_font_t>;
  using FontOptionsPtr = std::shared_ptr<cairo_font_options_t>;
  using FontExtentsPtr = std::shared_ptr<cairo_font_extents_t>;
  using TextExtentsPtr = std::shared_ptr<cairo_text_extents_t>;
  using GlyphVector = std::vector<cairo_glyph_t>;
  using GlyphVectorPtr = std::shared_ptr<GlyphVector>;

  Cairo() = delete;

  // TODO: Copy/move constructors/operators.
  explicit Cairo(unsigned char* buffer,
      const cairo_format_t colorFormat,
      const int width,
      const int height,
      const int stride);
  ~Cairo();

  void save();
  void restore();
  void clip(
      const double x, const double y, const double width, const double height);

  void clear(const Color& color = Color{});
  void drawLine(const double x1,
      const double y1,
      const double x2,
      const double y2,
      const double width,
      const Color& color = Color{});
  void showGlyphs(const GlyphVector& glyphs,
      const ScaledFontPtr scaledFont,
      const double x,
      const double y,
      const cairo_text_extents_t& extents,
      const Color& color);

  static constexpr int formatBitsPerPixel(cairo_format_t format);
  static ScaledFontPtr getScaledFont(
      const FT_Face ftFace, const int ftLoadFlags, const int pixelSize);
  static double xHeight(const ScaledFontPtr scaledFont);

  void rasterCopy(int diffX, int diffY);

private:
  template <typename StatusFunc, typename... Args>
  static void checkStatus(const StatusFunc statusFunc, Args&&... args);

  template <typename Ptr>
  static void checkPtrStatus(const Ptr ptr);

  ContextPtr mContext;
};  // class Cairo

// static
template <typename StatusFunc, typename... Args>
inline void Cairo::checkStatus(const StatusFunc statusFunc, Args&&... args)
{
  cairo_status_t status = statusFunc(std::forward<Args>(args)...);
  if(status != CAIRO_STATUS_SUCCESS) {
    std::string msg = "Cairo's status: ";
    msg += cairo_status_to_string(status);
    throw std::logic_error(msg);
  }
}

// static
template <typename Ptr>
inline void Cairo::checkPtrStatus(const Ptr ptr)
{
  if(!ptr) {
    std::string msg = "Cairo's status: ";
    msg += cairo_status_to_string(CAIRO_STATUS_NULL_POINTER);
    throw std::logic_error(msg);
  }
}

}  // namespace fg

#endif  // FG_CAIRO_H
