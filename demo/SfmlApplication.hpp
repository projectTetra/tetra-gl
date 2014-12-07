#ifndef TETRA_SFMLAPPLICATION_HPP
#define TETRA_SFMLAPPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <memory>

namespace tetra
{
class IGLResources
{
public:
  IGLResources();
  virtual ~IGLResources();

  virtual void render() = 0;
  virtual void onScreenResize( int, int ){};
};

/**
 * Basic SfmlApplicaion implementation. At the very least
 * you need to override the CreateOGLResources method.
 **/
class SfmlApp
{
public:
  /**
   * Creates the global EventManager.
   **/
  SfmlApp();

  /**
   * Runs the program.
   * 1) Calls ConstructWindow()
   * 2) Calls LoadOGLExtensions
   * 3) Calls CreateOGLResources
   * 4) Runs the MainLoop
   * 5) Any exceptions are handed to HandleTopLevelException
   **/
  void run();

protected:
  /**
   * Construct the sfml render window.
   * Defaults to a 800x600 window which supports OpenGL 3.1.
   **/
  virtual std::unique_ptr<sf::RenderWindow> constructWindow();

  /**
   * Returns the window name, defaults to "sfml window".
   **/
  virtual std::string windowName();

  /**
   * Loads all OpenGL functions. Defaults to using GLEW.
   **/
  virtual void loadOGLExtensions();

  /**
   * Create the IGLResources object.
   **/
  virtual std::unique_ptr<IGLResources> createOGLResources() = 0;

  /**
   * Handles all exceptions thrown out of the MainLoop or
   * the construction process. Defaults to pumping the output
   * to std::cerr and throwing a std::terminate.
   **/
  virtual void handleTopLevelException( const std::exception& e );

  /**
   * Main program loop. Runs a quickly as possible. Pulls
   * any available events and hands them to the HandleEvent
   * method. Updates the global event manager. Calls
   * IGLResource's Render method and swaps the window
   * buffers.
   **/
  virtual void mainLoop();

  /**
   * Handles SFML events. Defaults to killing the program if
   * Escape is pressed or if the window is closed.
   **/
  virtual void handleEvent( const sf::Event& event );

protected:
  std::unique_ptr<sf::RenderWindow> m_window;
  std::unique_ptr<IGLResources> m_resources;
  bool running{true};
};

} /* namespace tetra */

#endif
