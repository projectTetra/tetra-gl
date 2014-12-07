#include <tetra/gl/shaderProgram/Program.hpp>
#include <tetra/gl/GLException.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::shaderProgram;

Program::Program()
{
  this->handle = glCreateProgram();
  CheckGLError( "glCreateProgram" );
}

Program::~Program() NOEXCEPT { glDeleteProgram( this->handle ); }

Program::Program( Program&& move ) NOEXCEPT
{
  this->handle = move.handle;
  move.handle = 0;
}

Program& Program::operator=( Program&& move )
{
  glDeleteProgram( this->handle );
  CheckGLError( "glDeleteProgram" );

  this->handle = move.handle;
  move.handle = 0;

  return *this;
}

GLuint Program::expose() const NOEXCEPT { return this->handle; }

GLint Program::findUniform( const string& name ) const
{
  auto location = glGetUniformLocation( expose(), name.c_str() );
  CheckGLError( "glGetUniformLocation", {name} );

  return location;
}

GLint Program::findVertexAttrib( const string& name ) const
{
  auto location = glGetAttribLocation( expose(), name.c_str() );
  CheckGLError( "glGetAttribLocation", {name} );
  return location;
}

void Program::use() const
{
  glUseProgram( expose() );
  CheckGLError( "glUseProgram" );
}

