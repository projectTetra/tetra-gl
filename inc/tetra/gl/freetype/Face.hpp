#pragma once
#ifndef TETRA_GL_FREETYPE_FACE_HPP
#define TETRA_GL_FREETYPE_FACE_HPP

#include <tetra/gl/freetype/Library.hpp>
#include <tetra/gl/freetype/Glyph.hpp>
#include <string>
#include <map>

namespace tetra
{
namespace gl
{
namespace freetype
{

/**
 * RAII class for managing a freetype face.
 **/
class Face
{
  FT_Face face;
  std::map<unsigned int, Glyph> glyphCache;

public:
  /**
   * Attempts to load the font file into freetype face.
   * @throws FreeTypeException if the font cannot be loaded.
   **/
  Face( const std::string& fontFilePath, const Library& library );

  /**
   * Destroys the face when we are done with it.
   **/
  ~Face();

  /**
   * Exposes the underlying FT_Face object.
   **/
  FT_Face& expose() noexcept;
  const FT_Face& expose() const noexcept;

  /**
   * Loads the glyph from the font.
   **/
  Glyph loadGlyph( unsigned int glyph ) noexcept;
};

} /* namespace freetype */
} /* namespace gl */
} /* namespace tetra */

#endif
