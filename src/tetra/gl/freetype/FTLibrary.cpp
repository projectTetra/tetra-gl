#include <tetra/gl/freetype/Library.hpp>
#include <tetra/gl/freetype/FreeTypeException.hpp>

using namespace tetra;
using namespace tetra::gl::freetype;

Library::Library()
{
  auto error = FT_Init_FreeType( &library );
  if ( error == 0 )
  {
    throw FreeTypeException{"FT_Init_FreeType"};
  }
}

Library::~Library() { FT_Done_FreeType( library ); }

FT_Library& Library::expose() noexcept
{
  return library;
}

const FT_Library& Library::expose() const noexcept
{
  return library;
}
