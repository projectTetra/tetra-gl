#include <tetra/gl/freetype/Face.hpp>
#include <tetra/gl/freetype/FreeTypeException.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl::freetype;

Face::Face( const string& fontFilePath, const Library& library )
{
  auto error =
    FT_New_Face( library.expose(), fontFilePath.c_str(), 0, &face );

  if ( error == FT_Err_Unknown_File_Format )
  {
    throw FreeTypeException{"FT_New_Face",
                            {"FT_Err_Unknown_File_Format"}};
  }
  else if ( error )
  {
    throw FreeTypeException{"FT_New_Face"};
  }
}

Face::~Face() { FT_Done_Face( face ); }

FT_Face& Face::expose() noexcept
{
  return face;
}

const FT_Face& Face::expose() const noexcept
{
  return face;
}

Glyph Face::loadGlyph( unsigned int glyphId ) const noexcept
{
  FT_Load_Char( expose(), glyphId, FT_LOAD_RENDER );
  FT_GlyphSlot& glyph = expose()->glyph;
  FT_Glyph_Metrics& metrics = glyph->metrics;

  float xBearing = metrics.horiBearingX >> 6;
  float yBearing = ( metrics.horiBearingY - metrics.height ) >> 6;
  float height = metrics.height >> 6;
  float width = metrics.width >> 6;

  geometry::Rect bounds{yBearing, yBearing + height, xBearing,
                        xBearing + width};

  int bitmapRows = static_cast<int>( glyph->bitmap.rows );
  int bitmapWidth = static_cast<int>( glyph->bitmap.width );

  return Glyph( bounds, glyph->bitmap.buffer, bitmapRows, bitmapWidth,
                xBearing, yBearing, glyph->advance.x >> 6,
                glyph->advance.y >> 6 );
}

