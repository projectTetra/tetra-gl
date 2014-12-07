#pragma once
#ifndef TETRA_GL_GLEXCEPTION_HPP
#define TETRA_GL_GLEXCEPTION_HPP

#include <GL/glew.h>
#include <tetra/gl/TetraDef.hpp>
#include <stdexcept>
#include <vector>

namespace tetra
{
namespace gl
{

/**
 * Thrown when an OpenGL error is encountered.
 **/
class GLException : public std::logic_error
{
public:
  /**
   * Initializes the GLException with an error message including the
   * glFunctionName, a string representing the error, and all of the
   * additional detail messages provided.
   **/
  GLException(
    const std::string& glFunctionName, GLenum glError,
    const std::vector<std::string>& details = {} ) NOEXCEPT;

  /**
   * Returns the list of detail messages, good for testing if you are
   * expecting something to go wrong in a specific way.
   **/
  const std::vector<std::string>& getDetails() const NOEXCEPT;

  /**
   * Returns the function which caused the error.
   **/
  const std::string& getGLFunction() const NOEXCEPT;

  /**
   * Returns the string representing the OpenGL error code.
   **/
  std::string getGLErrorString() const NOEXCEPT;

  /**
   * Returns the actual GLenum error code that glGetError returned.
   **/
  GLenum getGLError() const NOEXCEPT;

private:
  std::string createErrMsg(
    const std::string& glFunctionName, GLenum glError,
    const std::vector<std::string>& details ) const NOEXCEPT;

  std::string findGLErrorString( GLenum err ) const NOEXCEPT;

private:
  std::string glFunctionName;
  std::vector<std::string> details;
  GLenum error;
};

/**
 * Calls glGetError, if there is an error this will throw a
 * GLException with the funcitonName and the detail messages
 *specified.
 **/
void CheckGLError( const std::string& glFunctionName,
                   const std::vector<std::string>& details = {} );

} /* namespace gl */
} /* namespace tetra */

#endif
