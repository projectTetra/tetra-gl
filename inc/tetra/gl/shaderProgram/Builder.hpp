#pragma once
#ifndef TETRA_GL_SHADERPROGRAM_BUILDER_HPP
#define TETRA_GL_SHADERPROGRAM_BUILDER_HPP

#include <map>
#include <vector>

#include <tetra/gl/shaderProgram/Program.hpp>
#include <tetra/gl/shaderProgram/Shader.hpp>

namespace tetra
{
namespace gl
{
namespace shaderProgram
{

/**
 * Builds a shaderProgram::Program using the parameters specified.
 **/
class Builder
{
public:
  /**
   * Loads the given file and prepares it to be compiled and linked to
   * the shader program.
   * @param filePath The path to the shader source code on disk.
   * @param type The type of the shader.
   * @throws runtime_error If the file cannot be found.
   **/
  Builder& addShaderFile( const std::string& filePath,
                          SHADER_TYPE type );

  /**
   * Prepares the source code to be compiled and linked to the shader
   * program.
   * @param source The shader's source code.
   * @param type The shader's type.
   **/
  Builder& addShaderSource( const std::string& source,
                            SHADER_TYPE type ) NOEXCEPT;

  /**
   * Binds the vertex attribute to the given index.
   * @param name The name of the vertex attribute to bind.
   * @param index The index to bind the vertex attribute to.
   **/
  Builder& bindVertexAttrib( const std::string& name,
                             GLuint index ) NOEXCEPT;

  /**
   * Binds the fragment shader's output the the given color attachment
   * index.
   * @param name The name of the fragment shader's output.
   * @param index The index of the color attachment to bind.
   **/
  Builder& bindFragDataLocation( const std::string& name,
                                 GLuint attachmentIndex ) NOEXCEPT;

  /**
   * Enables transform-feedback for a vertex shader output variable.
   * @param name The name of the vertex shader's output variable.
   **/
  Builder& enableVertexFeedback( const std::string& name ) NOEXCEPT;

  /**
   * Sets the vertex feedback mode, defaults to GL_SEPARATE_ATTRIBS.
   **/
  Builder& setTransformFeedbackMode( TRANSFORM_MODE mode ) NOEXCEPT;

  /**
   * Uses the data from the previous functions to build a shader
   * program.
   **/
  Program build();

private:
  std::vector<std::string> feedbackVaryings;
  GLenum feedbackBufferMode{GL_SEPARATE_ATTRIBS_EXT};

  Program program;
};

} /* namespace shaderProgram */
} /* namespace gl */
} /* namespace tetra */

#endif
