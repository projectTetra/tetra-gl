#pragma once
#ifndef TETRA_GL_SHADERPROGRAM_HPP
#define TETRA_GL_SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <string>

namespace tetra
{
namespace gl
{
namespace shaderProgram
{

/**
 * Type-Safe shader type enumerations.
 **/
enum class SHADER_TYPE
{
  VERTEX = GL_VERTEX_SHADER,
  FRAGMENT = GL_FRAGMENT_SHADER,
  COMPUTE = GL_COMPUTE_SHADER,
  GEOMETRY = GL_GEOMETRY_SHADER,
  TESS_CONTROL = GL_TESS_CONTROL_SHADER,
  TESS_EVAL = GL_TESS_EVALUATION_SHADER
};

/**
 * Type-Safe transform-feedback modes.
 **/
enum class TRANSFORM_MODE
{
  SEPARATE = GL_SEPARATE_ATTRIBS,
  INTERLEAVED = GL_INTERLEAVED_ATTRIBS
};

} /* namespace shaderProgram */
} /* namespace core */
} /* namespace tetra */

std::string
to_string( const tetra::gl::shaderProgram::SHADER_TYPE& type );

#endif
