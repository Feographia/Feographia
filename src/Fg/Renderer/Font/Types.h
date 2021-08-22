#ifndef FG_FONT_TYPES_H
#define FG_FONT_TYPES_H

#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace fg
{
namespace font
{
class Case;
class Font;

using FtLibraryPtr = std::shared_ptr<FT_LibraryRec_>;
using CasePtr = std::shared_ptr<Case>;
using FontPtr = std::shared_ptr<Font>;

}  // namespace font
}  // namespace fg

#endif  // FG_FONT_TYPES_H
