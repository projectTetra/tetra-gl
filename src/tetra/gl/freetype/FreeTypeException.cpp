#include <tetra/gl/freetype/FreeTypeException.hpp>

#include <sstream>

using namespace std;
using namespace tetra;
using namespace tetra::gl::freetype;

FreeTypeException::FreeTypeException( const string& fctnName,
                                      const vector<string>& details )
  : runtime_error{createErrorMessage( fctnName, details )}
{ }

string
FreeTypeException::createErrorMessage( const string& fctnName,
                                       const vector<string>& details )
{
  stringstream ss;
  ss << "Error in FreeType2 function: " << fctnName << "\n";

  if ( details.size() > 0 )
  {
    ss << "Details: \n";
    for ( const auto& detail : details )
    {
      ss << detail << "\n";
    }
  }

  return ss.str();
}
