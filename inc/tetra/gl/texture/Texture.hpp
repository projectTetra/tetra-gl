#pragma once
#ifndef TETRA_GL_TEXTURE_HPP
#define TETRA_GL_TEXTURE_HPP

#include <string>
#include <stdexcept>

#include <GL/glew.h>

#include <tetra/gl/TetraDef.hpp>
#include <tetra/gl/texture/ParameterTypes.hpp>

namespace tetra
{
namespace gl
{
namespace texture
{

/**
 * RAII class which manages an OpenGL texture.
 **/
class Texture
{
public:
  /**
   * Creates a single texture using glGenTextures.
   * @throws GLException if the call fails.
   **/
  Texture();

  /**
   * Destroys the texture using glDeleteTextures.
   * Does not check for errors.
   **/
  ~Texture();

  /**
   * Move constructor/operators.
   **/
  Texture( Texture&& Tex ) NOEXCEPT;
  Texture& operator=( Texture&& Tex ) NOEXCEPT;

  Texture( Texture& ) = delete;
  Texture& operator=( Texture& ) = delete;

  /**
   * Lowercase method name for standard-library conformance.
   **/
  void swap( Texture& toSwap ) NOEXCEPT;

  /**
   * Binds the texture to its texture unit.
   **/
  void bind() const;

  /**
   * Returns the handle to the OpenGL texture.
   **/
  GLuint expose() const NOEXCEPT;

  /**
   * Returns the texture unit that the texture will bind to.
   **/
  UNIT getUnit() const NOEXCEPT;

  /**
   * Returns the texture's type.
   **/
  TYPE getType() const NOEXCEPT;

  /**
   * Binds the texture and sets the proper uniform value.
   * @param loc The uniform location of the sampler in the
   *            current shader program.
   **/
  void setUniform( GLint loc ) const;

  /**
   * Queries OpenGL for all of the texture parameters.
   **/
  std::ostream& dumpParams( std::ostream& out );

private:
  GLuint handle;

  UNIT unit{UNIT::ZERO};
  TYPE type{TYPE::TEXTURE_2D};
};

/**
 * Queries OpenGL for all of the texture parameters.
 * Assumes that the texture of interest is currently bound and active.
 **/
std::ostream& DumpTextureParams( std::ostream& out, GLenum target );

} /* namespace texture */
} /* namespace gl */
} /* namespace tetra */

#endif
