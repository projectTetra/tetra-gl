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

Builder& Builder::AddShaderFile( const string& filePath,
                                 SHADER_TYPE type )
{
  return AddShaderSource( LoadTextFile( filePath, type ), type );
}

Builder& Builder::AddShaderSource( const string& src,
                                   SHADER_TYPE type ) NOEXCEPT
{
  auto shader = Shader{src, type};
  shader.Compile();

  glAttachShader( this->program.Expose(), shader.Expose() );
  CheckGLError( "glAttachShader", {to_string( shader.GetType() )} );

  return *this;
}

Builder& Builder::BindVertexAttrib( const string& name,
                                    GLuint index ) NOEXCEPT
{
  glBindAttribLocation( program.Expose(), index, name.c_str() );
  CheckGLError( "glBindAttribLocation", {name, to_string( index )} );

  return *this;
}

Builder&
Builder::BindFragDataLocation( const string& name,
                               GLuint attachmentIndex ) NOEXCEPT
{
  glBindFragDataLocation( this->program.Expose(), attachmentIndex,
                          name.c_str() );
  CheckGLError( "glBindFragDataLocation",
                {name, to_string( attachmentIndex )} );

  return *this;
}

Builder& Builder::EnableVertexFeedback( const string& name ) NOEXCEPT
{
  this->feedbackVaryings.emplace_back( name );
  return *this;
}

Builder&
Builder::SetTransformFeedbackMode( TRANSFORM_MODE mode ) NOEXCEPT
{
  this->feedbackBufferMode = static_cast<GLenum>( mode );
  return *this;
}

Program Builder::Build()
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
      this->program.Expose(), static_cast<GLsizei>( varArray.size() ),
      varArray.data(), this->feedbackBufferMode );
    CheckGLError( "glTransformFeedbackVaryings",
                  this->feedbackVaryings );
  }

  // Link the program
  glLinkProgram( this->program.Expose() );
  CheckGLError( "glLinkProgram" );

  // check the status of the link
  GLint testVal{GL_TRUE};
  glGetProgramiv( this->program.Expose(), GL_LINK_STATUS, &testVal );

  if ( testVal == GL_FALSE )
  {
    // the link failed, get the reason and throw
    const int logSize = 1024;
    char infoLog[logSize];
    glGetProgramInfoLog( this->program.Expose(), logSize, nullptr,
                         infoLog );

    throw ShaderCompileException{"Shader Program failed to link: " +
                                   string( infoLog ),
                                 SHADER_TYPE::VERTEX};
  }

  auto toRet = move( this->program );
  this->program = Program{};

  return move( toRet );
}

