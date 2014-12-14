#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/GLException.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>
#include <tetra/gl/texture/Configurer.hpp>
#include <tetra/gl/freetype/Face.hpp>
#include <tetra/gl/freetype/GlyphCache.hpp>
#include <tetra/gl/geometry/Rect.hpp>
#include <tetra/util/StopWatch.hpp>
#include <SOIL.h>
#include <SfmlApplication.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::util;

/**
 * We will hold Vertex information in this class, note that it has
 * standard layout.
 **/
class BasicVertex
{
public:
  BasicVertex( float x, float y, float z, float u = 0.0f,
               float v = 0.0f )
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
  freetype::GlyphCache glyphCache;
  util::Stopwatch<std::chrono::microseconds> timer;

public:
  /**
   * Here we will create all of our OpenGL resources.
   **/
  GLResources()
    : ftFace{"./demo/assets/Prototype.ttf", ftLibrary}
    , glyphCache{ftFace}
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

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    ftFace.setPixelSize( 48 );
  }

  void renderString( const string& str, float x, float y )
  {
    vector<BasicVertex> verticies;
    verticies.reserve( str.size() * 2 * 3 );

    glm::vec2 cursor{x, y};
    for ( const char& letter : str )
    {
      const auto& glyph = glyphCache.getGlyph( letter );
      float x = glyph.getXBearing() + cursor.x;
      float y = glyph.getYBearing() + cursor.y;
      float dx = glyph.getWidth();
      float dy = glyph.getRows();

      auto texCoords = glyphCache.getGlyphTexCoords( letter );

      // first triangle
      verticies.push_back(
        {x, y, 0.0f, texCoords.getLeft(), texCoords.getBottom()} );
      verticies.push_back( {x + dx, y, 0.0f, texCoords.getRight(),
                            texCoords.getBottom()} );
      verticies.push_back(
        {x, y + dy, 0.0f, texCoords.getLeft(), texCoords.getTop()} );

      // second triangle
      verticies.push_back( {x + dx, y, 0.0f, texCoords.getRight(),
                            texCoords.getBottom()} );
      verticies.push_back(
        {x, y + dy, 0.0f, texCoords.getLeft(), texCoords.getTop()} );
      verticies.push_back( {x + dx, y + dy, 0.0f,
                            texCoords.getRight(),
                            texCoords.getTop()} );

      cursor.x += glyph.getXAdvance();
      cursor.y += glyph.getYAdvance();
    }

    buffer.setData( move( verticies ) );
    buffer.draw( GL_TRIANGLES );
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

    glyphCache.setTileMapUniform( textureLocation );

    glm::vec4 vTextColor = glm::vec4{1.0f};
    glUniform4fv( textColorLocation, 1, &vTextColor[0] );

    timer.tic();
    static auto msg2 = string{"abcdefghijklmnopqrstuvwxy"};
    renderString( msg2, -100, 400 );
    renderString( msg2, -100, 300 );
    renderString( msg2, -100, 200 );
    renderString( msg2, -100, 100 );

    renderString( msg2, -100, -400 );
    renderString( msg2, -100, -300 );
    renderString( msg2, -100, -200 );
    renderString( msg2, -100, -100 );

    int ticCount = timer.count();

    renderString( "Time to render: " + to_string( ticCount ) +
                    " Microseconds",
                  -100, 500 );
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

