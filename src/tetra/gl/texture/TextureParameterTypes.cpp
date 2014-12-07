#include <tetra/gl/texture/ParameterTypes.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::texture;

string to_string( const texture::MAG_FILTER& filter )
{
  switch ( filter )
  {
  case texture::MAG_FILTER::NEAREST:
    return "NEAREST";

  case texture::MAG_FILTER::LINEAR:
    return "LINEAR";
  }
}

string to_string( const texture::MIN_FILTER& filter )
{
#define Case( type )                                                 \
  case texture::MIN_FILTER::type:                                    \
    return #type;

  switch ( filter )
  {
    Case( LINEAR );
    Case( NEAREST );
    Case( LINEAR_MIPMAP_LINEAR );
    Case( LINEAR_MIPMAP_NEAREST );
    Case( NEAREST_MIPMAP_NEAREST );
    Case( NEAREST_MIPMAP_LINEAR );
  }

#undef Case
}

string to_string( const texture::WRAP& wrap )
{
#define Case( type )                                                 \
  case WRAP::type:                                                   \
    return #type;

  switch(wrap)
  {
    Case( CLAMP_TO_EDGE );
    Case( REPEAT );
    Case( CLAMP_TO_BORDER );
    Case( MIRRORED_REPEAT );
    Case( MIRROR_CLAMP_TO_EDGE );
  }

#undef Case
}

string to_string( const texture::COMPARE_MODE& mode )
{
  switch( mode )
  {
    case texture::COMPARE_MODE::NONE:
      return "NONE";

    case texture::COMPARE_MODE::COMPARE_REF_TO_TEXTURE:
      return "COMPARE_REF_TO_TEXTURE";
    }
}

string to_string( const texture::COMPARE_FUNC& func )
{
#define Case( type )                                                 \
  case texture::COMPARE_FUNC::type:                                  \
    return #type;

  switch ( func )
  {
    Case( ALWAYS );
    Case( NEVER );
    Case( LESS );
    Case( EQUAL );
    Case( LEQUAL );
    Case( GREATER );
    Case( NOTEQUAL );
    Case( GEQUAL );
  }

#undef Case
}

string to_string( const texture::DEPTH_STENCIL_MODE& mode )
{
#define Case( type )                                                 \
  case texture::DEPTH_STENCIL_MODE::type:                            \
    return #type;

  switch( mode )
  {
    Case( DEPTH_COMPONENT );
    Case( STENCIL_COMPONENT );
  }

#undef Case
}

string to_string( const texture::UNIT& unit )
{
#define Case( unit )                                                 \
  case texture::UNIT::unit:                                          \
    return #unit;

  switch( unit )
  {
    Case( ZERO );
    Case( ONE );
    Case( TWO );
    Case( THREE );
    Case( FOUR );
    Case( FIVE );
    Case( SIX );
    Case( SEVEN );
    Case( EIGHT );
  }
#undef Case
}

int texture::UnitToInt( texture::UNIT unit )
{
#define Case( unit, i )                                              \
  case texture::UNIT::unit:                                          \
    return i;

  switch ( unit )
  {
    Case( ZERO, 0 );
    Case( ONE, 1 );
    Case( TWO, 2 );
    Case( THREE, 3 );
    Case( FOUR, 4 );
    Case( FIVE, 5 );
    Case( SIX, 6 );
    Case( SEVEN, 7 );
    Case( EIGHT, 8 );
  }

#undef Case
}

string to_string( const texture::TYPE& type )
{
#define Case( type )                                                 \
  case texture::TYPE::type:                                          \
    return #type;

  switch( type )
  {
    Case( TEXTURE_1D );
    Case( TEXTURE_2D );
    Case( TEXTURE_3D );
    Case( TEXTURE_1D_ARRAY );
    Case( TEXTURE_2D_ARRAY );
    Case( TEXTURE_RECTANGLE );
    Case( TEXTURE_CUBE_MAP );
  }
#undef Case
}

