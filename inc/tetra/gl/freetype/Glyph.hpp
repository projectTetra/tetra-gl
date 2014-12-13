#pragma once
#ifndef TETRA_GL_FREETYPE_GLYPH_HPP
#define TETRA_GL_FREETYPE_GLYPH_HPP

#include <tetra/gl/geometry/Rect.hpp>
#include <vector>

namespace tetra
{
namespace gl
{
namespace freetype
{

/**
 * Holds all of the information that we need to render a glyph, this
 * is _not_ a direct equivelent to the FT_Glyph in the freetype
 * library (but it holds much of the same info).
 **/
class Glyph
{
  const geometry::Rect boundingBox;
  const std::vector<unsigned char> bitmap;
  const int rows{0}, width{0};
  const float xBearing{0.0f}, yBearing{0.0f};
  const float xAdvance{0.0f}, yAdvance{0.0f};

public:
  Glyph() = default;
  Glyph( geometry::Rect bounds, unsigned char* bitmapData, int rows,
         int width, float xBearing, float yBearing, float xAdvance,
         float yAdvance );

  /**
   * Returns the smalled rectangle which completely surronuds the
   * glyph.
   **/
  const geometry::Rect& getBoundingBox() const noexcept;

  /**
   * Returns the vector holding the bitmap data. This is just
   * unsigned chars which represent the opacity.
   **/
  const std::vector<unsigned char>& getBitmap() const noexcept;

  /**
   * Returns the number of rows in the bitmap image.
   **/
  int getRows() const noexcept;

  /**
   * Returns the number of columns is the bitmaap image.
   **/
  int getWidth() const noexcept;

  /**
   * Returns the x-offset to use when rendering the glyph.
   **/
  float getXBearing() const noexcept;

  /**
   * Returns the y-offset to use when rendering the glyph.
   **/
  float getYBearing() const noexcept;

  /**
   * Returns the distance to move before rendering the next glyph.
   **/
  float getXAdvance() const noexcept;

  /**
   * Returns the distance to move before rendering the next glyph.
   **/
  float getYAdvance() const noexcept;
};

} /* namespace freetype */
} /* namespace gl */
} /* namespace tetra */

#endif
