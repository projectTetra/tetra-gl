#include <tetra/gl/texture/Texture.hpp>
#include <tetra/gl/GLException.hpp>
#include <iostream>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::texture;

Texture::Texture()
{
  glGenTextures( 1, &this->handle );
  CheckGLError( "glGenTextures" );
}

Texture::~Texture()
{
  // make sure the texture isn't bound before deleting it
  glActiveTexture( static_cast<GLenum>( this->unit ) );
  glBindTexture( static_cast<GLenum>( this->type ), 0 );

  glDeleteTextures( 1, &this->handle );
  this->handle = 0;
}

Texture::Texture( Texture&& toMove ) NOEXCEPT
{
  this->handle = toMove.handle;
  this->unit = toMove.unit;
  this->type = toMove.type;

  toMove.handle = 0;
}

Texture& Texture::operator=( Texture&& toMove ) NOEXCEPT
{
  swap( toMove );
  return *this;
}

void Texture::swap( Texture& toSwap ) NOEXCEPT
{
  std::swap( this->handle, toSwap.handle );
  std::swap( this->unit, toSwap.unit );
  std::swap( this->type, toSwap.type );
}

void Texture::Bind() const
{
  glActiveTexture( static_cast<GLenum>( this->unit ) );
  CheckGLError( "glActiveTexture", {to_string( this->unit )} );

  glBindTexture( static_cast<GLenum>( this->type ), this->handle );
  CheckGLError( "glBindTexture", {to_string( this->type )} );
}

GLuint Texture::Expose() const NOEXCEPT { return this->handle; }

UNIT Texture::GetUnit() const NOEXCEPT { return this->unit; }

TYPE Texture::GetType() const NOEXCEPT { return this->type; }

void Texture::SetUniform( GLint loc ) const
{
  this->Bind();
  glUniform1i( loc, UnitToInt( this->GetUnit() ) );
}

ostream& Texture::DumpParams( ostream& out )
{
  this->Bind();
  DumpTextureParams( out, static_cast<GLenum>( this->type ) );

  return out;
}

ostream& DumpTextureParams( ostream& out, GLenum target )
{
  GLint param = 0;
  float floatParam = 0.0f;
  float color[4];
#define dumpparam( name, type )                                      \
  glGetTexParameteriv( target, name, &param );                       \
  out << #name << ": " << to_string( static_cast<type>( param ) )    \
      << "\n";

  dumpparam( GL_TEXTURE_MAG_FILTER, texture::MAG_FILTER );
  dumpparam( GL_TEXTURE_MIN_FILTER, texture::MIN_FILTER );
  dumpparam( GL_TEXTURE_MIN_LOD, GLint );
  dumpparam( GL_TEXTURE_MAX_LOD, GLint );
  dumpparam( GL_TEXTURE_BASE_LEVEL, GLint );
  dumpparam( GL_TEXTURE_MAX_LEVEL, GLint );
  dumpparam( GL_TEXTURE_WRAP_S, texture::WRAP );
  dumpparam( GL_TEXTURE_WRAP_T, texture::WRAP );
  dumpparam( GL_TEXTURE_WRAP_R, texture::WRAP );
  dumpparam( GL_TEXTURE_RESIDENT, bool );
  dumpparam( GL_TEXTURE_COMPARE_MODE, texture::COMPARE_MODE );
  dumpparam( GL_TEXTURE_COMPARE_FUNC, texture::COMPARE_FUNC );
  dumpparam( GL_DEPTH_TEXTURE_MODE, texture::DEPTH_STENCIL_MODE );
  dumpparam( GL_GENERATE_MIPMAP, bool );

#undef dumpparam
  glGetTexParameterfv( target, GL_TEXTURE_BORDER_COLOR, color );
  out << "GL_TEXTURE_BORDER_COLOR: {" << color[0] << ", " << color[1]
      << ", " << color[2] << ", " << color[3] << "}\n";

  glGetTexParameterfv( target, GL_TEXTURE_PRIORITY, &floatParam );
  out << "GL_TEXTURE_PRIORITY: " << floatParam << endl;

  return out;
}

