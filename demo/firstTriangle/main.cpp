#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/GLException.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>

#include <SfmlApplication.hpp>

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
  BasicVertex( float x, float y ) : position{{x, y}} {};

  array<float, 2> position;
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
  GLResources() : passthrough{CreatePassthroughProgram()}
  {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

    buffer.EnableVertexAttrib( 0, &BasicVertex::position ); 
    buffer.SetData( {{-0.5f, -0.5f}, {0.0f, 0.5f}, {0.5f, -0.5f}} );
  }

  /**
   * Render the scene.
   **/
  void Render() override
  {
    glClear( GL_COLOR_BUFFER_BIT );
    passthrough.Use();
    buffer.Draw( GL_TRIANGLES );
  }

private:
  /**
   * Here we use the shaderProgram Builder to construct our shader
   * program instance.
   **/
  shaderProgram::Program CreatePassthroughProgram()
  {
    return shaderProgram::Builder{}
      .AddShaderFile( "./demo/shaders/passthrough.vert",
                      shaderProgram::SHADER_TYPE::VERTEX )
      .AddShaderFile( "./demo/shaders/passthrough.frag",
                      shaderProgram::SHADER_TYPE::FRAGMENT )
      .BindVertexAttrib( "vVertex", 0 )
      .Build();
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
  shaderProgram::Program passthrough;
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
  unique_ptr<IGLResources> CreateOGLResources() override
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
  myApp.Run(); // run the app!

  return 0;
}

