#pragma once
#ifndef TETRA_GL_TEXTURE_BUILDER_HPP
#define TETRA_GL_TEXTURE_BUILDER_HPP

#include <tetra/gl/texture/Texture.hpp>
#include <tetra/gl/TetraDef.hpp>

namespace tetra
{
namespace gl
{
namespace texture
{

/**
 * Simplifies the process of creating textures.
 **/
class Configurer
{
  Texture& texture;

public:
  /**
   * Binds the texture.
   **/
  Configurer( Texture& tex );

  /**
   * Loads an image from the file provided into the current
   * texture, automatically generates mipmaps.
   * This method will overwrite texture settings to accomodate
   * the image. (eg, filters are set to use mipmaps)
   **/
  Configurer& loadImage( const std::string& imagePath );

  /**
   * Sets the S texture wrapping parameter.
   **/
  Configurer& setWrapS( WRAP wrap );
  Configurer& setWrapT( WRAP wrap );
  Configurer& setWrapR( WRAP wrap );
};

/**
 * Thrown if there is an error while the configurer is
 * loading an image.
 **/
class ImageLoadingError : public std::runtime_error
{
public:
  ImageLoadingError( const std::string& filePath );
};

} /* namespace texture */
} /* namespace gl */
} /* namespace tetra */

#endif
