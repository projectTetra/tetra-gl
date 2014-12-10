#include <tetra/gl/freetype/Face.hpp>
#include <tetra/gl/freetype/FreeTypeException.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl::freetype;

Face::Face( const string& fontFilePath, const Library& library )
{
  auto error =
    FT_New_Face( library.expose(), fontFilePath.c_str(), 0, &face );

  if ( error == FT_Err_Unknown_File_Format )
  {
    throw FreeTypeException{"FT_New_Face",
                            {"FT_Err_Unknown_File_Format"}};
  }
  else if ( error )
  {
    throw FreeTypeException{"FT_New_Face"};
  }
}

Face::~Face() { FT_Done_Face( face ); }

FT_Face& Face::expose() noexcept
{
  return face;
}

const FT_Face& Face::expose() const noexcept
{
  return face;
}
