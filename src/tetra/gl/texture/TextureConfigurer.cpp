#include <tetra/gl/texture/Configurer.hpp>
#include <tetra/gl/GLException.hpp>
#include <SOIL.h>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::texture;

ImageLoadingError::ImageLoadingError( const std::string& filePath )
  : runtime_error{"Could not load image at path:  " + filePath}
{ }

Configurer::Configurer( Texture& tex ) : texture{tex}
{
  texture.bind();
  CheckGLError( "Configurer Constructor" );
}

Configurer& Configurer::loadImage( const string& filePath )
{
  if ( !SOIL_load_OGL_texture( filePath.c_str(), SOIL_LOAD_AUTO,
                               texture.expose(), SOIL_FLAG_MIPMAPS ) )
  {
    throw ImageLoadingError{filePath};
  }

  return *this;
}

Configurer& Configurer::setWrapS( WRAP wrap )
{
  glTexParameteri( static_cast<GLenum>( texture.getType() ),
                   GL_TEXTURE_WRAP_S, static_cast<GLint>( wrap ) );

  CheckGLError( "Set Wrapping",
                {"GL_TEXTURE_WRAP_S", to_string( wrap )} );
  return *this;
}

Configurer& Configurer::setWrapT( WRAP wrap )
{
  glTexParameteri( static_cast<GLenum>( texture.getType() ),
                   GL_TEXTURE_WRAP_T, static_cast<GLint>( wrap ) );

  CheckGLError( "Set Wrapping",
                {"GL_TEXTURE_WRAP_S", to_string( wrap )} );
  return *this;
}

Configurer& Configurer::setWrapR( WRAP wrap )
{
  glTexParameteri( static_cast<GLenum>( texture.getType() ),
                   GL_TEXTURE_WRAP_R, static_cast<GLint>( wrap ) );

  CheckGLError( "Set Wrapping",
                {"GL_TEXTURE_WRAP_S", to_string( wrap )} );
  return *this;
}

