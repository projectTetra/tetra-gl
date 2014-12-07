#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/GLException.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>
#include <tetra/gl/texture/Configurer.hpp>
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
public:
  /**
   * Here we will create all of our OpenGL resources.
   **/
  GLResources() : projectionProgram{CreatePassthroughProgram()}
  {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

    buffer.enableVertexAttrib( 0, &BasicVertex::position );
    buffer.enableVertexAttrib( 1, &BasicVertex::texCoords );
    buffer.setData( {{-5.0f, -5.0f, -75.0f, 0.0f, 0.0f},
                     {5.0f, -5.0f, -75.0f, 1.0f, 0.0f},
                     {-5.0f, 5.0f, -75.0f, 0.0f, 1.0f},
                     {5.0f, 5.0f, -75.0f, 1.0f, 1.0f}} );

    // Get the projection location
    projectionLocation =
      projectionProgram.findUniform( "projection" );

    textureLocation = projectionProgram.findUniform( "tex" );

    texture::Configurer{basicImage}
      .loadImage( "./demo/assets/basicTexture.png" )
      .setWrapS( texture::WRAP::REPEAT )
      .setWrapT( texture::WRAP::REPEAT )
      .setWrapR( texture::WRAP::REPEAT );

    CheckGLError( "GLResources Constructor",
                  {"setting texture parameters"} );

    texture::DumpTextureParams( cout, GL_TEXTURE_2D );
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

    buffer.draw( GL_TRIANGLE_STRIP );
  }

  /**
   * Called when the screen is resized, including at program start.
   **/
  void onScreenResize( int width, int height ) override
  {
    // When The screen is resized, we need to resize the viewport
    glViewport( 0, 0, width, height );

    // then rebuild the projection matrix
    float aspect =
      static_cast<float>( width ) / static_cast<float>( height );

    projectionMatrix =
      glm::perspective( 70.0f, aspect, 1.0f, 500.0f );
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

      .addShaderFile( "./demo/shaders/texture.frag",
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

