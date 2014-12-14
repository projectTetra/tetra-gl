#pragma once
#ifndef TETRA_GL_FREETYPE_FONTRENDERER_HPP
#define TETRA_GL_FREETYPE_FONTRENDERER_HPP

#include <tetra/gl/freetype/GlyphCache.hpp>
#include <tetra/gl/shaderProgram/Program.hpp>
#include <glm/glm.hpp>

namespace tetra
{
namespace gl
{
namespace freetype
{

/**
 * Uses a freetype::Face and a freetype::GlyphCache to efficiently
 * render text.
 **/
class FontRenderer
{
  Library library;
  Face face;
  GlyphCache glyphCache;

  shaderProgram::Program program;
  GLint colorLocation{-1};
  GLint textureLocation{-1};
  GLint projectionLocation{-1};
  glm::mat4 orthoProjection;

  std::array<float, 4> vTextColor{{1.0f, 1.0f, 1.0f, 1.0f}};

  /**
   * as text is drawn with the drawText() method, it is added to this
   * list. when finalize() is called, the text is processed and pushed
   * to the screen as a single draw call.
   **/
  std::vector<std::tuple<std::string, float, float>> textToFinalize;

public:
  /**
   * Creates a FontRenderer which can render text using the font
   * provided in the FontFilePath.
   **/
  FontRenderer( const std::string& fontFilePath, int pixelSize = 14 );

  /**
   * Draws text at the coordinates specified.
   * x,y = 0,0 is the center of the screen, coordinates are in pixels.
   **/
  void drawText( const std::string& text, float x, float y ) noexcept;

  /**
   * Pushes all drawn text onto the screen.
   * (if you don't call this, you won't see anything)
   **/
  void finalize() noexcept;

  /**
   * This should be called when the screen is resized as it will
   * update the projection matrix used to render text.
   **/
  void onScreenResize( int screenWidth, int screenHeight ) noexcept;

  /**
   * Sets the text color, values should be between 0-1.
   **/
  void setTextColor( float r, float g, float b, float a ) noexcept;

private:
  shaderProgram::Program createShaderProgram() const;
};

} /* namespace freetype */
} /* namespace gl */
} /* namespace tetra */

#endif
