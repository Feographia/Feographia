/*****************************************************************************
 * Project:  HtmlGrapheas
 * Purpose:  HTML text editor library
 * Author:   NikitaFeodonit, nfeodonit@yandex.com
 *****************************************************************************
 *   Copyright (c) 2017-2018 NikitaFeodonit
 *
 *    This file is part of the HtmlGrapheas project.
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

#ifndef FG_LITE_HTML_RENDERER_H
#define FG_LITE_HTML_RENDERER_H

#include <memory>
#include <string>

#include "litehtml/litehtml.h"

#include "Fg/Renderer/Cairo.h"
#include "Fg/Renderer/litehtml/LiteHtmlContainer.h"

namespace fg
{
class LiteHtmlRenderer
{
public:
  // TODO: Copy/move constructors/operators.
  explicit LiteHtmlRenderer();
  virtual ~LiteHtmlRenderer() = default;

  void createHtmlDocumentFromUtf8(const std::string& htmlText);
  int renderHtml(int width, int height);
  virtual void drawHtml(unsigned char* buffer,
      const cairo_format_t colorFormat,
      const int width,
      const int height,
      const int stride,
      const int htmlX,
      const int htmlY);

  void setBackgroundColor(const Color& color);
  void setBackgroundColor(const litehtml::web_color& color);

  LiteHtmlContainerPtr getContainer();
  std::shared_ptr<litehtml::context> getHtmlContext();
  litehtml::document::ptr getHtmlDocument();

private:
  Color mBackgroundColor;

  LiteHtmlContainerPtr mContainer;
  std::shared_ptr<litehtml::context> mHtmlContext;
  litehtml::document::ptr mHtmlDocument;

  CairoPtr mCairo;

  unsigned char* mBuffer;
  int mBufferWidth;
  int mBufferHeight;
  int mBufferStride;
  int mHtmlX;
  int mHtmlY;
};

inline void LiteHtmlRenderer::setBackgroundColor(const Color& color)
{
  mBackgroundColor = color;
}

inline void LiteHtmlRenderer::setBackgroundColor(
    const litehtml::web_color& color)
{
  mBackgroundColor = LiteHtmlContainer::getColorFromWebColor(color);
}

inline LiteHtmlContainerPtr LiteHtmlRenderer::getContainer()
{
  return mContainer;
}
inline std::shared_ptr<litehtml::context> LiteHtmlRenderer::getHtmlContext()
{
  return mHtmlContext;
}

inline litehtml::document::ptr LiteHtmlRenderer::getHtmlDocument()
{
  return mHtmlDocument;
}

}  // namespace fg

#endif  // FG_LITE_HTML_RENDERER_H
