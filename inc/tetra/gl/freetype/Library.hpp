#pragma once
#ifndef TETRA_GL_FREETYPE_LIBRARY_HPP
#define TETRA_GL_FREETYPE_LIBRARY_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tetra
{
namespace gl
{
namespace freetype
{

/**
 * RAII wrapper for the freetype2 library object.
 **/
class Library
{
  FT_Library library;

public:
  /**
   * Initializes the library.
   * @throws FreeTypeException if initialization fails.
   **/
  Library();

  /**
   * Destroys the library.
   **/
  ~Library();

  /**
   * Exposes the raw FT_Library to the caller.
   **/
  FT_Library& expose() noexcept;
  const FT_Library& expose() const noexcept;
};

} /* namespace freetype */
} /* namespace gl */
} /* namespace tetra */

#endif
