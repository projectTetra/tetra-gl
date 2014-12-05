#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/GLException.hpp>
#include <SfmlApplication.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;

class BasicVertex
{
public:
  BasicVertex( float x, float y ) : position{{x, y}} {};

  array<float, 2> position;
};

class GLResources : public IGLResources
{
public:
  GLResources() : passthrough{CreatePassthroughProgram()}
  {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

    this->buffer.EnableVertexAttrib( 0, &BasicVertex::position );

    this->buffer.SetData(
      {{-0.5f, -0.5f}, {0.0f, 0.5f}, {0.5f, -0.5f}} );
  }

  void Render() override
  {
    glClear( GL_COLOR_BUFFER_BIT );
    this->passthrough.Use();
    this->buffer.Draw( GL_TRIANGLES );
  }

private:
  shaderProgram::Program CreatePassthroughProgram()
  {
    return shaderProgram::Builder{}
      .AddShaderFile( "./demo/passthrough.vert",
                      shaderProgram::SHADER_TYPE::VERTEX )
      .AddShaderFile( "./demo/passthrough.frag",
                      shaderProgram::SHADER_TYPE::FRAGMENT )
      .BindVertexAttrib( "vVertex", 0 )
      .Build();
  }

private:
  Buffer<BasicVertex> buffer;
  shaderProgram::Program passthrough;
};

class App : public SfmlApp
{
public:
  unique_ptr<IGLResources> CreateOGLResources() override
  {
    glGetError();
    return unique_ptr<IGLResources>{new GLResources{}};
  }
};

int main()
{
  App myApp{};
  myApp.Run();

  return 0;
}

