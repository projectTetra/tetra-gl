#include <tetra/gl/freetype/GlyphCache.hpp>
#include <tetra/gl/texture/Configurer.hpp>
#include <tetra/gl/GLException.hpp>
#include <glm/glm.hpp>
#include <iostream>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::freetype;

GlyphCache::GlyphCache( Face& face ) : face{&face}
{
  texture::Configurer{glyphTileMap}
    .setMagFilter( texture::MAG_FILTER::NEAREST )
    .setMinFilter( texture::MIN_FILTER::NEAREST )
    .setWrapR( texture::WRAP::CLAMP_TO_EDGE )
    .setWrapS( texture::WRAP::CLAMP_TO_EDGE )
    .setWrapT( texture::WRAP::CLAMP_TO_EDGE );

  CheckGLError( "Glyph Cache C'tor" );
}

void GlyphCache::addGlyph( unsigned int glyphId ) noexcept
{
  auto iter = glyphCacheMap.find( glyphId );
  if ( iter != end( glyphCacheMap ) )
  {
    return;
  }

  glyphCacheMap[glyphId] = GlyphDesc{
    unique_ptr<Glyph>{new Glyph{face->loadGlyph( glyphId )}},
    geometry::Rect{}};

  rebuildGlyphTileMap();
}

const Glyph& GlyphCache::getGlyph( unsigned int glyphId ) noexcept
{
  addGlyph( glyphId );

  auto glyphIter = glyphCacheMap.find( glyphId );
  return *glyphIter->second.glyph;
}

geometry::Rect
GlyphCache::getGlyphTexCoords( unsigned int glyphId ) noexcept
{
  addGlyph( glyphId );

  auto glyphIter = glyphCacheMap.find( glyphId );
  return glyphIter->second.texCoords;
}

void GlyphCache::setTileMapUniform( int uniformLocation ) noexcept
{
  glyphTileMap.setUniform( uniformLocation );
}

void GlyphCache::clearCache() noexcept
{
  glyphCacheMap.clear();
}

void GlyphCache::rebuildGlyphTileMap() noexcept
{
  auto totalBounds = calcPaddedBoundingBox();
  float totalWidth = totalBounds.getRight() - totalBounds.getLeft();
  float totalHeight = totalBounds.getTop() - totalBounds.getBottom();

  glyphTileMap.bind();
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, totalWidth, totalHeight, 0,
                GL_RED, GL_UNSIGNED_BYTE, 0 );
  CheckGLError( "teximage 2d " );

  int penX = 0;
  for ( auto& glyphDesc : glyphCacheMap )
  {
    Glyph& glyph = *glyphDesc.second.glyph;
    glTexSubImage2D( GL_TEXTURE_2D, 0, penX, 0, glyph.getWidth(),
                     glyph.getRows(), GL_RED, GL_UNSIGNED_BYTE,
                     glyph.getBitmap().data() );

    geometry::Rect& texCoords = glyphDesc.second.texCoords;
    texCoords.setLeft( penX / totalWidth );
    texCoords.setRight( ( penX + glyph.getWidth() ) / totalWidth );
    texCoords.setBottom( glyph.getRows() / totalHeight );
    texCoords.setTop( 0.0f );

    penX += 1 + glyph.getWidth();
  }
}

geometry::Rect GlyphCache::calcPaddedBoundingBox() const noexcept
{
  geometry::Rect bBox{};
  int penX = 0;

  for ( const auto& glyphDesc : glyphCacheMap )
  {
    geometry::Rect glyphBound{};
    glyphBound.setBottom( 0.0f );
    glyphBound.setTop( glyphDesc.second.glyph->getRows() );
    glyphBound.setLeft( penX );
    glyphBound.setRight( penX + glyphDesc.second.glyph->getWidth() );

    bBox = geometry::BoundingBox( bBox, glyphBound );

    penX += 1 + glyphDesc.second.glyph->getWidth();
  }

  return bBox;
}



