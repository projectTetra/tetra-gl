#pragma once
#ifndef TETRA_GL_BASE_PROGRAM_HPP
#define TETRA_GL_BASE_PROGRAM_HPP

#include <string>

#include <GL/glew.h>

#include <tetra/gl/TetraDef.hpp>

namespace tetra
{
namespace gl
{
namespace shaderProgram
{

/**
 * RAII class for managing an OpenGL shader program.
 **/
class Program
{
public:
  /**
   * Uses glCreateProgram to create a shader program.
   * @throws GLException if the glCreateProgram causes an error.
   **/
  Program();

  /**
   * Deletes the program using glDeleteProgram. Does not check for
   * errors.
   **/
  ~Program() NOEXCEPT;

  /**
   * Move constructor.
   * @param move The program to be moved from.
   **/
  Program( Program&& move ) NOEXCEPT;

  /**
   * Move operator, steals the moved-from object's shader program.
   * Delete's its current program before stealing.
   **/
  Program& operator=( Program&& );

  Program( Program& Copy ) = delete;
  Program& operator=( Program& Copy ) = delete;

  /**
   * Exposes the OpenGL handle to the shader program.
   **/
  GLuint expose() const NOEXCEPT;

  /**
   * Finds the vertex attribute in this Program.
   * @param name The name of the vertex attribute to find.
   * @return The vertex attribute's location.
   * @throws GLExecption if the call causes an error.
   **/
  GLint findVertexAttrib( const std::string& name ) const;

  /**
   * Finds the uniform location in this Program.
   * @param name The name of a uniform to find.
   * @return The uniform's location.
   * @throws GLException if the call causes an error.
   **/
  GLint findUniform( const std::string& name ) const;

  /**
   * Uses this program for rendering.
   * @throws GLException if the call causes an error.
   **/
  void use() const;

private:
  GLuint handle;
};

} /* namespace shaderProgram */
} /* namespace gl */
} /* namespace tetra */

#endif
