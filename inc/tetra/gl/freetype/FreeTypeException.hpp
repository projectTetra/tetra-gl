#pragma once
#ifndef TETRA_GL_FREETYPE_FREETYPEEXCEPTION_HPP
#define TETRA_GL_FREETYPE_FREETYPEEXCEPTION_HPP

#include <stdexcept>
#include <vector>

namespace tetra
{
namespace gl
{
namespace freetype
{

/**
 * Thrown when there is an issue with a FreeType function call.
 **/
class FreeTypeException : public std::runtime_error
{
  public:
    FreeTypeException( const std::string& fctnName,
                       const std::vector<std::string>& details = {} );

  private:
    std::string
    createErrorMessage( const std::string& fctnName,
                        const std::vector<std::string>& details );
};

} /* namespace freetype */
} /* namespace gl */
} /* namespace tetra */

#endif
