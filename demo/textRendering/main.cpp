#include <tetra/gl/freetype/FontRenderer.hpp>
#include <tetra/util/StopWatch.hpp>
#include <SfmlApplication.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::util;

/**
 * IGLResources is an interface defined in the SfmlApplication header.
 * This object will be managed so that its lifetime is a subset of the
 * window's lifetime. (so we can create and destroy OpenGL-based
 * resources in the constructor and destructor)
 **/
class GLResources : public IGLResources
{
  freetype::FontRenderer fontRenderer;
  util::Stopwatch<std::chrono::microseconds> timer;

public:
  /**
   * Here we will create all of our OpenGL resources.
   **/
  GLResources() : fontRenderer{"./demo/assets/Prototype.ttf", 48}
  {
    glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  }

  /**
   * Render the scene.
   **/
  void render() override
  {
    glClear( GL_COLOR_BUFFER_BIT );

    timer.tic();
    static auto msg2 = string{"abcdefghijklmnopqrstuvwxy"};
    fontRenderer.drawText( msg2, -100, 400 );
    fontRenderer.drawText( msg2, -100, 300 );
    fontRenderer.drawText( msg2, -100, 200 );
    fontRenderer.drawText( msg2, -100, 100 );
    fontRenderer.finalize();

    int ticCount = timer.count();

    fontRenderer.drawText( "Time to render: " +
                             to_string( ticCount ) + " microseconds",
                           -100, 500 );
    fontRenderer.finalize();
  }

  /**
   * Called when the screen is resized, including at program start.
   **/
  void onScreenResize( int width, int height ) override
  {
    // When The screen is resized, we need to resize the viewport
    glViewport( 0, 0, width, height );

    fontRenderer.onScreenResize( width, height );
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

