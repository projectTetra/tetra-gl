#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/GLException.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>

#include <SfmlApplication.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;

class GLResources : public IGLResources
{
public:
  /**
   * Here we will create all of our OpenGL resources.
   **/
  GLResources()
  {
    glClearColor( 0.1f, 0.1f, 0.3f, 1.0f );
  }

  /**
   * Render the scene.
   **/
  void render() override
  {
    glClear( GL_COLOR_BUFFER_BIT );
  }
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

