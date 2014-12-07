#include <tetra/gl/shaderProgram/Shader.hpp>
#include <tetra/gl/GLException.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::shaderProgram;

ShaderCompileException::ShaderCompileException( const string& msg,
                                                SHADER_TYPE type )
  : runtime_error( createErrorMessage( "", type, msg ) )
  , errorMessage{msg}
  , completeMessage( createErrorMessage( "", type, msg ) )
  , type{type}
{ }

const char* ShaderCompileException::what() const NOEXCEPT
{
  return this->completeMessage.c_str();
}

void
ShaderCompileException::setShaderName( const string& name ) NOEXCEPT
{
  this->shaderName = name;
  this->completeMessage =
    createErrorMessage( name, this->type, this->errorMessage );
}

const string& ShaderCompileException::getShaderName() const NOEXCEPT
{
  return this->shaderName;
}

string ShaderCompileException::createErrorMessage(
  const string& shaderName, SHADER_TYPE type,
  const string& error ) NOEXCEPT
{
  return "Compile failed for " + to_string( type ) + " shader: " +
         shaderName + ", " + error;
}

Shader::Shader( const string& Source, SHADER_TYPE type ) : type{type}
{
  this->handle = glCreateShader( static_cast<GLuint>( type ) );
  CheckGLError( "glCreateShader", {to_string( type )} );

  auto Cptr = Source.c_str();
  glShaderSource( this->handle, 1, &Cptr, nullptr );
  CheckGLError( "glShaderSource", {to_string( type )} );
}

Shader::~Shader() NOEXCEPT
{
  glDeleteShader( this->handle );
  this->handle = 0;
}

Shader::Shader( Shader&& move ) NOEXCEPT
{
  this->type = move.type;
  this->handle = move.handle;
  move.handle = 0;
}

Shader& Shader::operator=( Shader&& move )
{
  glDeleteShader( this->handle );
  CheckGLError( "glDeleteShader", {to_string( this->type )} );

  this->handle = move.handle;
  move.handle = 0;

  swap( this->type, move.type );

  return *this;
}

void Shader::compile() const
{
  GLint testVal = GL_FALSE;

  glCompileShader( this->handle );
  CheckGLError( "glCompileShader", {to_string( this->type )} );

  glGetShaderiv( this->handle, GL_COMPILE_STATUS, &testVal );
  CheckGLError( "glGetShaderiv", {to_string( this->type )} );

  // check the compile status
  if ( testVal == GL_FALSE )
  {
    const int logSize = 1024;
    char log[logSize];
    glGetShaderInfoLog( this->handle, logSize, nullptr, log );
    throw ShaderCompileException{log, this->type};
  }
}

SHADER_TYPE Shader::getType() const NOEXCEPT { return this->type; }

GLuint Shader::expose() const NOEXCEPT { return this->handle; }

