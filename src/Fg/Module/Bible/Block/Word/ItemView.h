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

#ifndef FG_ITEM_VIEW_H
#define FG_ITEM_VIEW_H

#include <vector>

#include "Fg/Module/Bible/Base/View.h"
#include "Fg/Module/Bible/Block/Word/WordPartView.h"

namespace fg
{
class ItemView : public View
{
public:
  using View::View;
  using View::operator=;

private:
  std::vector<WordPartView> mWordParts;
};

}  // namespace fg

#endif  // FG_ITEM_VIEW_H
