#include <GL/glew.h>
#include <SfmlApplication.hpp>

#include <stdexcept>
#include <iostream>

using namespace std;
using namespace tetra;

IGLResources::IGLResources(){};

IGLResources::~IGLResources(){};

SfmlApp::SfmlApp() {}

void SfmlApp::Run()
{
  try
  {
    m_window = ConstructWindow();
    LoadOGLExtensions();
    m_resources = CreateOGLResources();

    // make sure to resize based on the _actual_ screen size
    auto size = m_window->getSize();
    m_resources->OnScreenResize( size.x, size.y );

    MainLoop();
  }
  catch ( std::exception& e )
  {
    HandleTopLevelException( e );
  }
}

std::unique_ptr<sf::RenderWindow> SfmlApp::ConstructWindow()
{
  auto settings = sf::ContextSettings{};
  settings.antialiasingLevel = 2;
  settings.depthBits = 24;
  settings.majorVersion = 3;
  settings.minorVersion = 1;
  settings.stencilBits = 8;

  auto vm = sf::VideoMode{800, 600};
  return std::unique_ptr<sf::RenderWindow>{new sf::RenderWindow{
    vm, WindowName(), sf::Style::Default, settings}};
}

std::string SfmlApp::WindowName() { return "sfml window"; }

void SfmlApp::LoadOGLExtensions()
{
  glewExperimental = true;
  auto Status = glewInit();
  if ( Status != GLEW_OK )
  {
    auto err =
      string{"GLEW initialization failure: "} +
      reinterpret_cast<const char*>( glewGetErrorString( Status ) );

    throw runtime_error{err};
  }
}

void SfmlApp::HandleTopLevelException( const std::exception& e )
{
  std::cerr << "Fatal Error: " << e.what() << std::endl;
  std::terminate();
}

void SfmlApp::MainLoop()
{
  while ( running )
  {
    auto event = sf::Event{};
    // process events
    while ( m_window->pollEvent( event ) )
    {
      HandleEvent( event );
    }

    m_resources->Render();

    // then swap buffers
    m_window->display();
  }
}

void SfmlApp::HandleEvent( const sf::Event& event )
{
  // if the user presses the x button, then quit
  switch ( event.type )
  {
  case sf::Event::Closed:
    running = false;
    break;

  case sf::Event::KeyPressed:
    if ( event.key.code == sf::Keyboard::Escape )
    {
      running = false;
    }
    break;

  case sf::Event::Resized:
    m_resources->OnScreenResize( event.size.width,
                                 event.size.height );
    break;

  default:
    break;
  }
}
