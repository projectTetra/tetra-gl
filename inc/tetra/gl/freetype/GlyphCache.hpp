#pragma once
#ifndef TETRA_GL_FREETYPE_GLYPHCACHE_HPP
#define TETRA_GL_FREETYPE_GLYPHCACHE_HPP

#include <tetra/gl/freetype/Face.hpp>
#include <tetra/gl/texture/Texture.hpp>

#include <map>
#include <memory>

namespace tetra
{
namespace gl
{
namespace freetype
{

/**
 * Cache's glyphs in the FreeType Face so that they don't have to be
 * re-rendered and streamed into graphics memory on every render.
 **/
class GlyphCache
{
  /**
   * Holds a glyph and its texture coordinates in the glyphTileMap.
   **/
  struct GlyphDesc
  {
    std::unique_ptr<Glyph> glyph;
    geometry::Rect texCoords;
  };

  texture::Texture glyphTileMap;
  std::map<unsigned int, GlyphDesc> glyphCacheMap;
  freetype::Face* face{nullptr};

public:
  GlyphCache() = default;
  GlyphCache( freetype::Face& face );

  /**
   * Adds the glyph associated with the glyphId to the cache.
   * If the glyph is already in the cache, this is a no-op.
   **/
  void addGlyph( unsigned int glyphId ) noexcept;

  /**
   * Gets the Glyph object associated with the glyphId.
   **/
  const Glyph& getGlyph( unsigned int glyphId ) noexcept;

  /**
   * Gets the texture coordinates of the glyph in the glyphTileMap.
   **/
  geometry::Rect getGlyphTexCoords( unsigned int glyph ) noexcept;

  /**
   * Binds the glyphTileMap and sets the texture uniform in the
   * currently bound shaderProgram.
   **/
  void setTileMapUniform( int uniformLocation ) noexcept;

  /**
   * Clears the glyph cache, this is used when something happens to
   * make the cached glyphs become invalid. (for example, if you call
   * FT_Set_Pixel_Size on the face, you will need to clear the cache
   * or else you will not see the right letters).
   **/
  void clearCache() noexcept;

private:
  /**
   * Called when a new glyph is added to the tilemap.
   * Rebuilds the glyphTileMap to hold all of the glyph images.
   **/
  void rebuildGlyphTileMap() noexcept;

  /**
   * Calculates the dimensions of a bounding box which surrounds all
   * letters in the cache. Accounts for a 1px padding between letters.
   **/
  geometry::Rect calcPaddedBoundingBox() const noexcept;
};

} /* namespace freetype */
} /* namespace gl */
} /* namespace tetra */

#endif
