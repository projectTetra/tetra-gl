#include <tetra/gl/GLException.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;

void tetra::gl::CheckGLError( const string& glFunctionName,
                              const vector<string>& details )
{
  GLenum err = glGetError();
  if ( err != GL_NO_ERROR )
    throw GLException{glFunctionName, err, details};
}

GLException::GLException( const string& glFunctionName,
                          GLenum glError,
                          const vector<string>& details ) NOEXCEPT
  : logic_error{CreateErrMsg( glFunctionName, glError, details )},
    glFunctionName{glFunctionName},
    details{details},
    error{glError}
{ }

const vector<string>& GLException::GetDetails() const NOEXCEPT
{
  return this->details;
}

const string& GLException::GetGLFunction() const NOEXCEPT
{
  return this->glFunctionName;
}

std::string GLException::GetGLErrorString() const NOEXCEPT
{
  return FindGLErrorString( this->error );
}

GLenum GLException::GetGLError() const NOEXCEPT
{
  return this->error;
}

string GLException::CreateErrMsg(
  const string& glFunctionName, GLenum glError,
  const vector<string>& details ) const NOEXCEPT
{
  auto errMsg =
    glFunctionName + ": " + FindGLErrorString( glError ) + "\n";
  for ( const auto& detail : details )
  {
    errMsg += detail + ", \n";
  }

  return errMsg;
}

string GLException::FindGLErrorString( GLenum err ) const NOEXCEPT
{
#define Case( type )                                                 \
  case type:                                                         \
    return #type;

  switch ( err )
  {
    Case( GL_NO_ERROR );
    Case( GL_INVALID_ENUM );
    Case( GL_INVALID_VALUE );
    Case( GL_INVALID_OPERATION );
    Case( GL_INVALID_FRAMEBUFFER_OPERATION_EXT );
    Case( GL_OUT_OF_MEMORY );
  default:
    return "unknown";
  }

#undef Case
}

