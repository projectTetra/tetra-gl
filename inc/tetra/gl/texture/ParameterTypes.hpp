#pragma once
#ifndef TETRA_GL_TEXTURE_ENUMS_HPP
#define TETRA_GL_TEXTURE_ENUMS_HPP

#include <GL/glew.h>
#include <string>

namespace tetra
{
namespace gl
{
namespace texture
{

/**
 * Texture type enumeration.
 **/
enum class TYPE
{
  TEXTURE_1D = GL_TEXTURE_1D,
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_3D = GL_TEXTURE_3D,
  TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
  TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
  TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
  TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
};

/**
 *  * Texture unit UNIT enumeration.
 *   **/
enum class UNIT
{
  ZERO = GL_TEXTURE0,
  ONE = GL_TEXTURE1,
  TWO = GL_TEXTURE2,
  THREE = GL_TEXTURE3,
  FOUR = GL_TEXTURE4,
  FIVE = GL_TEXTURE5,
  SIX = GL_TEXTURE6,
  SEVEN = GL_TEXTURE7,
  EIGHT = GL_TEXTURE8
};

/**
 * Texture Depth-Stencil mode enumeration.
 **/
enum class DEPTH_STENCIL_MODE
{
  DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
  STENCIL_COMPONENT = GL_STENCIL_COMPONENTS
};

/**
 * Texture comparison function enumeration.
 **/
enum class COMPARE_FUNC
{
  LEQUAL = GL_LEQUAL,
  GEQUAL = GL_GEQUAL,
  LESS = GL_LESS,
  GREATER = GL_GREATER,
  EQUAL = GL_EQUAL,
  NOTEQUAL = GL_NOTEQUAL,
  ALWAYS = GL_ALWAYS,
  NEVER = GL_NEVER
};

/**
 * Texture comparison mode enumeration.
 **/
enum class COMPARE_MODE
{
  COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
  NONE = GL_NONE
};

/**
 * Texture minification filter enumeration.
 **/
enum class MIN_FILTER
{
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

/**
 * Texture magnification filter enumeration.
 **/
enum class MAG_FILTER
{
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR
};

/**
 *  * Texture component swizzle policy enumeration.
 *   **/
enum class SWIZZLE
{
  RED = GL_RED,
  GREEN = GL_GREEN,
  BLUE = GL_BLUE,
  ALPHA = GL_ALPHA,
  ZERO = GL_ZERO,
  ONE = GL_ONE
};

/**
 *  * Texture coordinate wrapping policy enumeration.
 *   **/
enum class WRAP
{
  CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  REPEAT = GL_REPEAT,
  MIRROR_CLAMP_TO_EDGE =
      GL_MIRROR_CLAMP_TO_EDGE // OpenGL 4.4 and later
};

GLint UnitToInt( tetra::gl::texture::UNIT Unit );

} /* namespace texture */
} /* namespace gl */
} /* namespace tetra */

std::string to_string( const tetra::gl::texture::MAG_FILTER& filter );

std::string to_string( const tetra::gl::texture::MIN_FILTER& filter );

std::string to_string( const tetra::gl::texture::WRAP& wrap );

std::string to_string( const tetra::gl::texture::COMPARE_MODE& mode );

std::string to_string( const tetra::gl::texture::COMPARE_FUNC& func );

std::string to_string(
  const tetra::gl::texture::DEPTH_STENCIL_MODE& depthStencilMode );

std::string to_string( const tetra::gl::texture::UNIT& unit );

std::string to_string( const tetra::gl::texture::TYPE& type );

#endif
