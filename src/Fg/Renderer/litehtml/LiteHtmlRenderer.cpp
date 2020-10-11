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

#include "Fg/Renderer/litehtml/LiteHtmlRenderer.h"

#include <cassert>
#include <cstdlib>

//#include <chrono>
//#include <iostream>

namespace fg
{
LiteHtmlRenderer::LiteHtmlRenderer()
    : mBackgroundColor(1, 1, 1, 1)
    , mContainer {std::make_shared<LiteHtmlContainer>()}
    , mHtmlContext {std::make_shared<litehtml::context>()}
    , mHtmlDocument {nullptr}
    , mCairo {nullptr}
    , mBufferWidth {0}
    , mBufferHeight {0}
    , mBufferStride {0}
    , mHtmlX {0}
    , mHtmlY {0}
{
}

void LiteHtmlRenderer::createHtmlDocumentFromUtf8(const std::string& htmlText)
{
  mHtmlDocument = litehtml::document::createFromUTF8(
      htmlText.c_str(), mContainer.get(), mHtmlContext.get());
}

int LiteHtmlRenderer::renderHtml(int width, int height)
{
  mContainer->setDeviceWidth(width);
  mContainer->setDeviceHeight(height);
  mContainer->setDisplayAreaWidth(width);
  mContainer->setDisplayAreaHeight(height);

  // Render HTML document.
  int bestWidth = mHtmlDocument->render(width);
  assert(bestWidth != 0);
  return bestWidth;
}

void LiteHtmlRenderer::drawHtml(
    unsigned char* buffer,
    const cairo_format_t colorFormat,
    const int width,
    const int height,
    const int stride,
    const int htmlX,
    const int htmlY)
{
  // https://stackoverflow.com/a/18685338
  //auto start = std::chrono::steady_clock::now();

  litehtml::uint_ptr hdcCairo = reinterpret_cast<litehtml::uint_ptr>(&mCairo);

  bool fullDraw = width != mBufferWidth || height != mBufferHeight
      || stride != mBufferStride || abs(mHtmlX - htmlX) >= width
      || abs(mHtmlY - htmlY) >= height;

  if(fullDraw) {
    mCairo =
        std::make_shared<Cairo>(buffer, colorFormat, width, height, stride);

    mCairo->save();
    mCairo->clear(mBackgroundColor);
    litehtml::position testClip(0, 0, width, height);
    mHtmlDocument->draw(hdcCairo, -htmlX, -htmlY, &testClip);
    mCairo->restore();

  } else {
    int diffX = htmlX - mHtmlX;
    int diffY = htmlY - mHtmlY;

    mCairo->rasterCopy(diffX, diffY);

    int x1 = 0;
    int y1 = 0;
    int x2 = width;
    int y2 = height;

    if(diffX != 0) {
      if(diffX > 0) {
        x1 = x2 - diffX;
      } else {
        x2 = -diffX;  // x2 = x1 - diffX, but x1 == 0;
      }
      if(x1 != x2) {
        int clipWidth = x2 - x1;
        int clipHeight = y2 - y1;
        mCairo->save();
        mCairo->clip(x1, y1, clipWidth, clipHeight);
        mCairo->clear(mBackgroundColor);
        litehtml::position textClip(x1, y1, clipWidth, clipHeight);
        mHtmlDocument->draw(hdcCairo, -htmlX, -htmlY, &textClip);
        mCairo->restore();
      }
    }

    if(diffY != 0) {
      if(width == x2 && 0 != x1) {
        x2 = x1;
        x1 = 0;
      } else if(0 == x1 && width != x2) {
        x1 = x2;
        x2 = width;
      }

      if(diffY > 0) {
        y1 = y2 - diffY;
      } else {
        y2 = -diffY;  // y2 = y1 - diffY, but y1 == 0;
      }
      if(y1 != y2) {
        int clipWidth = x2 - x1;
        int clipHeight = y2 - y1;
        mCairo->save();
        mCairo->clip(x1, y1, clipWidth, clipHeight);
        mCairo->clear(mBackgroundColor);
        litehtml::position textClip(x1, y1, clipWidth, clipHeight);
        mHtmlDocument->draw(hdcCairo, -htmlX, -htmlY, &textClip);
        mCairo->restore();
      }
    }
  }

  if(width != mBufferWidth) {
    mBufferWidth = width;
  }
  if(height != mBufferHeight) {
    mBufferHeight = height;
  }
  if(stride != mBufferStride) {
    mBufferStride = stride;
  }
  if(htmlX != mHtmlX) {
    mHtmlX = htmlX;
  }
  if(htmlY != mHtmlY) {
    mHtmlY = htmlY;
  }

  //auto finish = std::chrono::steady_clock::now();
  //auto elapsed =
  //    std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
  //std::cout << "HtmlRenderer::drawHtml: " << elapsed.count() << "\n";
}

}  // namespace fg
