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

Glyph Face::loadGlyph( unsigned int glyphId ) noexcept
{
  FT_Load_Char( expose(), glyphId, FT_LOAD_RENDER );
  FT_GlyphSlot& glyphSlot = expose()->glyph;
  FT_Glyph_Metrics& metrics = glyphSlot->metrics;

  float xBearing = metrics.horiBearingX >> 6;
  float yBearing = ( metrics.horiBearingY - metrics.height ) >> 6;
  float height = metrics.height >> 6;
  float width = metrics.width >> 6;

  geometry::Rect bounds{yBearing, yBearing + height, xBearing,
                        xBearing + width};

  int bitmapRows = static_cast<int>( glyphSlot->bitmap.rows );
  int bitmapWidth = static_cast<int>( glyphSlot->bitmap.width );

  return Glyph( bounds, glyphSlot->bitmap.buffer, bitmapRows,
                bitmapWidth, xBearing, yBearing,
                glyphSlot->advance.x >> 6,
                glyphSlot->advance.y >> 6 );
}

void Face::setPixelSize( int size ) noexcept
{
  FT_Set_Pixel_Sizes( expose(), 0, size );
}

