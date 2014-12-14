#include <tetra/gl/freetype/FontRenderer.hpp>
#include <tetra/gl/shaderProgram/Builder.hpp>
#include <tetra/gl/Buffer.hpp>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::freetype;
using namespace tetra::gl::shaderProgram;

namespace
{

struct BasicVertex
{
  BasicVertex() = default;
  BasicVertex( float x, float y, float u, float v )
    : vVertex{{x, y}}, vTexCoords{{u, v}} {};

  array<float, 2> vVertex;
  array<float, 2> vTexCoords;
};

using VertexBuffer = Buffer<BasicVertex>;

VertexBuffer createVertexBuffer()
{
  VertexBuffer buf;
  buf.enableVertexAttrib( 0, &BasicVertex::vVertex );
  buf.enableVertexAttrib( 1, &BasicVertex::vTexCoords );
  return move( buf );
}

/**
 * Adds the vertex data for the string given to the vertex list.
 **/
void addTextVerticies( vector<BasicVertex>& verticies,
                       GlyphCache& glyphCache, const string& text, float x,
                       float y )
{
  glm::vec2 cursor{x, y};
  for ( const char& letter : text )
  {
    const auto& glyph = glyphCache.getGlyph( letter );
    float x = glyph.getXBearing() + cursor.x;
    float y = glyph.getYBearing() + cursor.y;
    float dx = glyph.getWidth();
    float dy = glyph.getRows();

    auto texCoords = glyphCache.getGlyphTexCoords( letter );

    // first triangle
    verticies.push_back(
      {x, y, texCoords.getLeft(), texCoords.getBottom()} );
    verticies.push_back(
      {x + dx, y, texCoords.getRight(), texCoords.getBottom()} );
    verticies.push_back(
      {x, y + dy, texCoords.getLeft(), texCoords.getTop()} );

    // second triangle
    verticies.push_back(
      {x + dx, y, texCoords.getRight(), texCoords.getBottom()} );
    verticies.push_back(
      {x, y + dy, texCoords.getLeft(), texCoords.getTop()} );
    verticies.push_back(
      {x + dx, y + dy, texCoords.getRight(), texCoords.getTop()} );

    cursor.x += glyph.getXAdvance();
    cursor.y += glyph.getYAdvance();
  }
}
} /* namespace */

FontRenderer::FontRenderer( const string& fontFilePath,
                            int pixelSize )
  : face{fontFilePath, library}
  , glyphCache{face}
  , program{createShaderProgram()}
{
  face.setPixelSize( pixelSize );

  colorLocation = program.findUniform( "vTextColor" );
  textureLocation = program.findUniform( "tex" );
  projectionLocation = program.findUniform( "projection" );
}

void FontRenderer::drawText( const std::string& text, float x,
                             float y ) noexcept
{
  for ( const char& letter : text )
  {
    glyphCache.addGlyph( letter );
  }

  textToFinalize.push_back( make_tuple( text, x, y ) );
}

void FontRenderer::finalize() noexcept
{
  static VertexBuffer vertexBuffer = createVertexBuffer();

  vector<BasicVertex> verticies;

  for ( const auto& textDescriptor : textToFinalize )
  {
    addTextVerticies( verticies, glyphCache, get<0>( textDescriptor ),
                      get<1>( textDescriptor ),
                      get<2>( textDescriptor ) );
  }
  textToFinalize.clear();

  program.use();
  glyphCache.setTileMapUniform( textureLocation );
  glUniform4fv( colorLocation, 1, vTextColor.data() );
  glUniformMatrix4fv( projectionLocation, 1, GL_FALSE,
                      &orthoProjection[0][0] );

  vertexBuffer.setData( move( verticies ) );
  vertexBuffer.draw( GL_TRIANGLES );
}

void FontRenderer::onScreenResize( int width, int height ) noexcept
{
  float halfWidth = static_cast<float>( width ) / 2.0f;
  float halfHeight = static_cast<float>( height ) / 2.0f;

  orthoProjection = glm::ortho( -halfWidth, halfWidth, -halfHeight,
                                halfHeight, 1.0f, -1.0f );
}

void FontRenderer::setTextColor( float r, float g, float b,
                                 float a ) noexcept
{
  vTextColor = {{r, g, b, a}};
}

shaderProgram::Program FontRenderer::createShaderProgram() const
{
  return shaderProgram::Builder{}
    .addShaderFile( "./demo/shaders/projection.vert",
                    SHADER_TYPE::VERTEX )

    .addShaderFile( "./demo/shaders/textRender.frag",
                    SHADER_TYPE::FRAGMENT )
    .bindVertexAttrib( "vVertex", 0 )
    .bindVertexAttrib( "vTexCoords", 1 )
    .build();
}
