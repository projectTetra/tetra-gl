#pragma once
#ifndef TETRA_GL_BASE_VERTEXARRAYOBJECT_HPP
#define TETRA_GL_BASE_VERTEXARRAYOBJECT_HPP

#include <GL/glew.h>
#include <tetra/gl/TetraDef.hpp>

namespace tetra
{
namespace gl
{

/**
 * RAII class for managing a VertexArrayObject.
 **/
class VertexArrayObject
{
public:
  /**
   * Creates an OpenGL VAO.
   * @throws GLException if glGenVertexArrays fails
   **/
  VertexArrayObject();

  /**
   * Deletes the OpenGL VAO.
   **/
  ~VertexArrayObject() NOEXCEPT;

  /**
   * VertexArrayObjects can be moved but not copied.
   **/
  VertexArrayObject( VertexArrayObject&& Move ) NOEXCEPT;
  VertexArrayObject& operator=( VertexArrayObject&& Move ) NOEXCEPT;

  VertexArrayObject( VertexArrayObject& Copy ) = delete;
  VertexArrayObject& operator=( VertexArrayObject& Copy ) = delete;

  /**
   * Calls GLBindVertexArray to bind the vertex array.
   **/
  void Bind() const NOEXCEPT;

  /**
   * Exposes the OpenGL handle to the vertex array.
   * Does not relinquish ownership of the handle.
   **/
  GLuint Expose() const NOEXCEPT;

private:
  GLuint handle;
};

} /* namespace gl */
} /* namespace tetra */

#endif
