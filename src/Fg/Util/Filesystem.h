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

#ifndef FG_FILESYSTEM_H
#define FG_FILESYSTEM_H

#ifdef USE_BOOST
#include <boost/filesystem.hpp>
#else  // #ifndef USE_BOOST
#include <filesystem>
#endif  // #ifdef USE_BOOST

namespace fg
{
#ifdef USE_BOOST
#ifndef BOOST_FILESYSTEM_NO_DEPRECATED
#define BOOST_FILESYSTEM_NO_DEPRECATED
#endif
namespace filesystem = boost::filesystem;
#else  // #ifndef USE_BOOST
namespace filesystem = std::filesystem;
#endif  // #ifdef USE_BOOST
}  // namespace fg

#endif  // FG_FILESYSTEM_H
