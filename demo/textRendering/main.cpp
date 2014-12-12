#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/GLException.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>
#include <tetra/gl/texture/Configurer.hpp>
#include <tetra/gl/freetype/Face.hpp>
#include <SOIL.h>
#include <SfmlApplication.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace std;
using namespace tetra;
using namespace tetra::gl;

/**
 * We will hold Vertex information in this class, note that it has
 * standard layout.
 **/
class BasicVertex
{
public:
  BasicVertex( float x, float y, float z, float u, float v )
    : position{{x, y, z}}, texCoords{{u, v}}
  { }

  array<float, 3> position;
  array<float, 2> texCoords;
};

/**
 * IGLResources is an interface defined in the SfmlApplication header.
 * This object will be managed so that its lifetime is a subset of the
 * window's lifetime. (so we can create and destroy OpenGL-based
 * resources in the constructor and destructor)
 **/
class GLResources : public IGLResources
{
  freetype::Library ftLibrary;
  freetype::Face ftFace;
public:
  /**
   * Here we will create all of our OpenGL resources.
   **/
  GLResources()
    : ftFace{"./demo/assets/Prototype.ttf", ftLibrary}
    , projectionProgram{CreatePassthroughProgram()}
  {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    buffer.enableVertexAttrib( 0, &BasicVertex::position );
    buffer.enableVertexAttrib( 1, &BasicVertex::texCoords );

    // Get the projection location
    projectionLocation =
      projectionProgram.findUniform( "projection" );

    textureLocation = projectionProgram.findUniform( "tex" );
    textColorLocation = projectionProgram.findUniform( "vTextColor" );

    texture::Configurer{basicImage}
      .setWrapS( texture::WRAP::REPEAT )
      .setWrapT( texture::WRAP::REPEAT )
      .setWrapR( texture::WRAP::REPEAT )
      .setMinFilter( texture::MIN_FILTER::LINEAR )
      .setMagFilter( texture::MAG_FILTER::LINEAR );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  }

  void renderString( const string& str )
  {
    FT_Set_Pixel_Sizes( ftFace.expose(), 0, 48 );

    glm::vec2 cursor{ 0.0f, 0.0f };

    for (const char& letter : str )
    {
      FT_Load_Char( ftFace.expose(), letter, FT_LOAD_RENDER );
      FT_GlyphSlot glyph = ftFace.expose()->glyph;

      basicImage.bind();
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width,
                    glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                    glyph->bitmap.buffer );

      CheckGLError( "Render Letter", {{letter}} );

      float x = (glyph->metrics.horiBearingX >> 6) + cursor.x;
      float y =
        ( ( glyph->metrics.horiBearingY - glyph->metrics.height ) >>
          6 ) +
        cursor.y;

      float dx = static_cast<float>( glyph->bitmap.width );
      float dy = static_cast<float>( glyph->bitmap.rows );

      buffer.setData( {{x, y, 0.0f, 0.0f, 1.0f},
                       {x + dx, y, 0.0f, 1.0f, 1.0f},
                       {x, y + dy, 0.0f, 0.0f, 0.0f},
                       {x + dx, y + dy, 0.0f, 1.0f, 0.0f}} );
      buffer.draw( GL_TRIANGLE_STRIP );

      cursor.x += (glyph->advance.x >> 6);
      cursor.y += (glyph->advance.y >> 6);
    }
  }

  /**
   * Render the scene.
   **/
  void render() override
  {
    glClear( GL_COLOR_BUFFER_BIT );
    projectionProgram.use();

    // Set the projection matrix before drawing the buffer
    glUniformMatrix4fv( projectionLocation, 1, GL_FALSE,
                        &projectionMatrix[0][0] );

    basicImage.setUniform( textureLocation );

    glm::vec4 vTextColor = glm::vec4{1.0f};
    glUniform4fv( textColorLocation, 1, &vTextColor[0] );

    renderString( "{[(pTest)]}" );
  }

  /**
   * Called when the screen is resized, including at program start.
   **/
  void onScreenResize( int width, int height ) override
  {
    // When The screen is resized, we need to resize the viewport
    glViewport( 0, 0, width, height );
    float fWidth = static_cast<float>(width)/2.0f;
    float fHeight = static_cast<float>(height)/2.0f;

    projectionMatrix =
      glm::ortho( -fWidth, fWidth, -fHeight, fHeight, 1.0f, -1.0f );
  }

private:
  /**
   * Here we use the shaderProgram Builder to construct our shader
   * program instance.
   **/
  shaderProgram::Program CreatePassthroughProgram()
  {
    return shaderProgram::Builder{}
      .addShaderFile( "./demo/shaders/projection.vert",
                      shaderProgram::SHADER_TYPE::VERTEX )

      .addShaderFile( "./demo/shaders/textRender.frag",
                      shaderProgram::SHADER_TYPE::FRAGMENT )

      .bindVertexAttrib( "vVertex", 0 )
      .bindVertexAttrib( "vTexCoords", 1 )
      .build();
  }

private:
  /**
   * Vertex buffer holding BasicVertex structs.
   * We will use the EnableVertexAttrib method to map the BasicVertex
   * attributes.
   **/
  Buffer<BasicVertex> buffer;

  /**
   * ShaderProgram, we will just use this to pass verticies and
   * fragments through the pipeline.
   **/
  shaderProgram::Program projectionProgram;

  /**
   * We need to hold the location of the projection uniform, and we
   * need to hold the actual projection matrix.
   **/
  GLint projectionLocation{-1};
  glm::mat4 projectionMatrix{1.0f};

  GLint textureLocation{-1};
  texture::Texture basicImage;

  GLint textColorLocation{-1};
};

/**
 * Main application object.
 **/
class App : public SfmlApp
{
public:
  /**
   * Create our instance of the GLResources class to be managed by the
   * SfmlApp object.
   **/
  unique_ptr<IGLResources> createOGLResources() override
  {
    glGetError(); // make sure that the error flag is clear
    return unique_ptr<IGLResources>{new GLResources{}};
  }
};

/**
 * Application entry point.
 **/
int main()
{
  App myApp{};
  myApp.run(); // run the app!

  return 0;
}

