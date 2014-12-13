#include <tetra/gl/freetype/Glyph.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::freetype;

Glyph::Glyph( geometry::Rect bounds, unsigned char* bitmapData,
              int rows, int width, float xBearing, float yBearing,
              float xAdvance, float yAdvance )
  : boundingBox{bounds}
  , bitmap( bitmapData, bitmapData + ( width * rows ) )
  , rows{rows}
  , width{width}
  , xBearing{xBearing}
  , yBearing{yBearing}
  , xAdvance{xAdvance}
  , yAdvance{yAdvance} {};

const geometry::Rect& Glyph::getBoundingBox() const noexcept
{
  return boundingBox;
}

const vector<unsigned char>& Glyph::getBitmap() const noexcept
{
  return bitmap;
}

int Glyph::getRows() const noexcept { return rows; }

int Glyph::getWidth() const noexcept { return width; }

float Glyph::getXBearing() const noexcept { return xBearing; }

float Glyph::getYBearing() const noexcept { return yBearing; }

float Glyph::getXAdvance() const noexcept { return xAdvance; }

float Glyph::getYAdvance() const noexcept { return yAdvance; }
