#pragma once
#ifndef TETRA_GL_BUFFER_HPP
#define TETRA_GL_BUFFER_HPP

#include <array>
#include <vector>

#include <tetra/gl/VertexArrayObject.hpp>
#include <tetra/gl/GLException.hpp>

namespace tetra
{
namespace gl
{

/**
* A safe, RAII OpenGL array buffer object.
* Templated on the type of the data that the buffer should
* contain. The buffer also keeps an internal VertexArrayObject
* which OpenGL uses to keep track of the enabled vertex attributes
* and how to get the attribute data.
**/
template <class VertexType>
class Buffer
{
public:
  /**
   * Creates a new buffer using glGenBuffers.
   * @throws GLException if glGenBuffers fails.
   **/
  Buffer();

  /**
   * Automatically deletes underlying OpenGL buffer.
   **/
  ~Buffer();

  /**
   * Move constructor.
   **/
  Buffer( Buffer<VertexType>&& b ) NOEXCEPT;

  /**
  * Move assignment, deletes the current buffer and steals
  * the input's buffer.
  * @throws GLException if glDeleteBuffers fails during the copy
  **/
  Buffer<VertexType>& operator=( Buffer<VertexType>&& b );

  /**
   * Sets the buffer's data and the vertex count.
   * @param data A vector of vertex data
   * @param BufferUsage GLenum used to provide a hint to OpenGL abuot
   *        how the buffer will be used.
   * @throws GLException if there are any gl errors
   **/
  void SetData( const std::vector<VertexType>& data,
                GLenum BufferUsage = GL_STATIC_DRAW );

  /**
   * Sets the index data when drawing indexed geometry.
   * Creates an element array buffer if it did not already
   * exist. VertexCount will now store the number of
   * indicies, rather than the size of the ArrayBuffer.
   * @param data A vector of vertex indicies
   * @param BufferUsage GLenum used to provide a hint to OpenGL about
   *        how the buffer will be used.
   * @throws GLException if there are any gl errors
   **/
  void SetElementData( const std::vector<unsigned short>& data,
                       GLenum BufferUsage = GL_STATIC_DRAW );

  /**
   * Enables the vertex attribute with the index specified.
   * @param Index The index of the attribute to enable
   * @param Attrib Pointer to a std::array of float data
   *               which represents the attribute in the
   *               VertexType structure.
   * @throws GLException if there are any OpenGL errors
   **/
  template <std::array<float, 1>::size_type length>
  Buffer<VertexType>&
  EnableVertexAttrib( GLuint Index,
                      std::array<float, length> VertexType::*Attrib );

  /**
   * Draws the vertex data using the topology specified.
   * If there is an element buffer then this will call glDrawElements,
   * otherwise it will call glDrawArrays.
   * @throws GLException if there are any OpenGL errors
   **/
  void Draw( GLenum Topology = GL_TRIANGLES ) const;

  /**
  * Returns the handle to the buffer.
  * Note: do not delete the buffer handle!
  **/
  GLuint Expose() const NOEXCEPT;

  /**
   * Binds the VertexArrayObject.
   **/
  void Bind() const NOEXCEPT;

  /**
   * Returns the number of verticies in this buffer. (the
   * size of the vector in the last SetData call)
   **/
  unsigned int Size() const NOEXCEPT;

private:
  Buffer( Buffer& ) = delete;
  Buffer& operator=( Buffer& ) = delete;

private:
  GLuint bufferHandle{0};
  GLuint elementBufferHandle{0};
  unsigned int vertexCount{0};

  VertexArrayObject vao;
};

#include <tetra/gl/buffer/BufferImpl.hpp>
} /* namespace gl */
} /* namespace tetra */

#endif
