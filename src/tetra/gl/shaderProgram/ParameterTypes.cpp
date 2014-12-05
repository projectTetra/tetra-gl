#include <tetra/gl/shaderProgram/ParameterTypes.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::gl;
using namespace tetra::gl::shaderProgram;

string to_string( const SHADER_TYPE& type )
{
#define Case( val )                                                  \
  case SHADER_TYPE::val:                                             \
    return #val;

  switch ( type )
  {
    Case( VERTEX );
    Case( FRAGMENT );
    Case( GEOMETRY );
    Case( COMPUTE );
    Case( TESS_CONTROL );
    Case( TESS_EVAL );
  }

#undef Case
}
