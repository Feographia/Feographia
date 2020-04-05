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

#ifndef FG_TYPES_H
#define FG_TYPES_H

#include <string>

namespace fg
{
using String = std::string;
using Coordinate = double;

enum class FontStyle
{
  fontStyleNormal,
  fontStyleItalic,
};

struct Color
{
  explicit Color()
      : mRed{0.0}
      , mGreen{0.0}
      , mBlue{0.0}
      , mAlpha{1.0}
  {
  }

  explicit Color(const double red,
      const double green,
      const double blue,
      const double alpha)
      : mRed{red}
      , mGreen{green}
      , mBlue{blue}
      , mAlpha{alpha}
  {
  }

  double mRed;
  double mGreen;
  double mBlue;
  double mAlpha;
};

}  // namespace fg

#endif  // FG_TYPES_H
