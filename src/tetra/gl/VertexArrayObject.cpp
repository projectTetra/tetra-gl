#include <tetra/gl/VertexArrayObject.hpp>
#include <tetra/gl/GLException.hpp>

using namespace tetra;
using namespace tetra::gl;

VertexArrayObject::VertexArrayObject()
{
  glGenVertexArrays( 1, &this->handle );
  CheckGLError( "glGenVertexArrays" );
}

VertexArrayObject::~VertexArrayObject() throw()
{
  glDeleteVertexArrays( 1, &this->handle );
  CheckGLError( "glDeleteVertexArrays", {"dtor"} );
}

VertexArrayObject::VertexArrayObject(
  VertexArrayObject&& Move ) throw()
{
  this->handle = Move.handle;
  Move.handle = 0;
}

VertexArrayObject& VertexArrayObject::
operator=( VertexArrayObject&& Move ) throw()
{
  glDeleteVertexArrays( 1, &this->handle );
  CheckGLError( "glDeleteVertexArrays", {"copy operator"} );

  this->handle = Move.handle;
  Move.handle = 0;

  return *this;
}

void VertexArrayObject::Bind() const NOEXCEPT
{
  glBindVertexArray( this->handle );
  CheckGLError( "glBindVertexArray" );
}

GLuint VertexArrayObject::Expose() const NOEXCEPT
{
  return this->handle;
}

