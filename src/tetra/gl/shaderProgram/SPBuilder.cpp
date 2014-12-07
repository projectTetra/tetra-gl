#include <tetra/gl/shaderProgram/Builder.hpp>
#include <tetra/gl/GLException.hpp>

#include <fstream>
#include <sstream>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::shaderProgram;

namespace
{
string LoadTextFile( const string& fileName, SHADER_TYPE type )
{
  ifstream InFile( fileName, ios::in );
  if ( !InFile.is_open() )
  {
    throw ShaderCompileException{"Could not open file " + fileName,
                                 type};
  }

  stringstream ss;
  string s;
  while ( getline( InFile, s ) )
  {
    ss << s << endl;
  }

  return ss.str();
}
}

Builder& Builder::addShaderFile( const string& filePath,
                                 SHADER_TYPE type )
{
  return addShaderSource( LoadTextFile( filePath, type ), type );
}

Builder& Builder::addShaderSource( const string& src,
                                   SHADER_TYPE type ) NOEXCEPT
{
  auto shader = Shader{src, type};
  shader.compile();

  glAttachShader( this->program.expose(), shader.expose() );
  CheckGLError( "glAttachShader", {to_string( shader.getType() )} );

  return *this;
}

Builder& Builder::bindVertexAttrib( const string& name,
                                    GLuint index ) NOEXCEPT
{
  glBindAttribLocation( program.expose(), index, name.c_str() );
  CheckGLError( "glBindAttribLocation", {name, to_string( index )} );

  return *this;
}

Builder&
Builder::bindFragDataLocation( const string& name,
                               GLuint attachmentIndex ) NOEXCEPT
{
  glBindFragDataLocation( this->program.expose(), attachmentIndex,
                          name.c_str() );
  CheckGLError( "glBindFragDataLocation",
                {name, to_string( attachmentIndex )} );

  return *this;
}

Builder& Builder::enableVertexFeedback( const string& name ) NOEXCEPT
{
  this->feedbackVaryings.emplace_back( name );
  return *this;
}

Builder&
Builder::setTransformFeedbackMode( TRANSFORM_MODE mode ) NOEXCEPT
{
  this->feedbackBufferMode = static_cast<GLenum>( mode );
  return *this;
}

Program Builder::build()
{
  // Set feedback varyings if specified
  unsigned numVaryings = this->feedbackVaryings.size();
  if ( numVaryings )
  {
    auto varArray = vector<const char*>( numVaryings );
    for ( unsigned i = 0; i < numVaryings; ++i )
    {
      varArray[i] = this->feedbackVaryings[i].c_str();
    }

    glTransformFeedbackVaryings(
      this->program.expose(), static_cast<GLsizei>( varArray.size() ),
      varArray.data(), this->feedbackBufferMode );

    CheckGLError( "glTransformFeedbackVaryings",
                  this->feedbackVaryings );
  }

  // Link the program
  glLinkProgram( this->program.expose() );
  CheckGLError( "glLinkProgram" );

  // check the status of the link
  GLint testVal{GL_TRUE};
  glGetProgramiv( this->program.expose(), GL_LINK_STATUS, &testVal );

  if ( testVal == GL_FALSE )
  {
    // the link failed, get the reason and throw
    const int logSize = 1024;
    char infoLog[logSize];
    glGetProgramInfoLog( this->program.expose(), logSize, nullptr,
                         infoLog );

    throw ShaderCompileException{"Shader Program failed to link: " +
                                   string( infoLog ),
                                 SHADER_TYPE::VERTEX};
  }

  auto toRet = move( this->program );
  this->program = Program{};

  return move( toRet );
}

