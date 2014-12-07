#pragma once
#ifndef TETRA_GL_BASE_SHADER_HPP
#define TETRA_GL_BASE_SHADER_HPP

#include <GL/glew.h>

#include <stdexcept>

#include <tetra/gl/shaderProgram/ParameterTypes.hpp>
#include <tetra/gl/TetraDef.hpp>

namespace tetra
{
namespace gl
{
namespace shaderProgram
{

/**
 * RAII class for managing OpenGL shaders.
 **/
class Shader
{
public:
  /**
   * Creates a new OpenGL Shader, and sets its source code.
   * @param Source The shader's source code
   * @param Type The type of the shader.
   * @throws GLException if there is an error when creating the shader
   **/
  Shader( const std::string& source, SHADER_TYPE type );

  /**
   * Deletes the OpenGL Shader.
   **/
  ~Shader() NOEXCEPT;

  /**
   * Move constructor.
   **/
  Shader( Shader&& move ) NOEXCEPT;
  Shader& operator=( Shader&& move );

  Shader( const Shader& ) = delete;
  Shader& operator=( const Shader& ) = delete;

  /**
   * Compiles the shader object, throws a ShaderCompileException if
   * the shader fails to compile.
   **/
  void compile() const;

  /**
   * @return This shader's type.
   **/
  SHADER_TYPE getType() const NOEXCEPT;

  /**
   * Exposes the handle to be manipulated externally, but the Shader
   * still "owns" the OpenGL shader.
   **/
  GLuint expose() const NOEXCEPT;

private:
  GLuint handle; //!< OpenGL handle to the shader
  SHADER_TYPE type;
};

/**
 * Thrown if a shader fails to compile.
 **/
class ShaderCompileException : public std::runtime_error
{
public:
  ShaderCompileException( const std::string& msg, SHADER_TYPE type );

  const char* what() const NOEXCEPT override;

  void setShaderName( const std::string& name ) NOEXCEPT;

  const std::string& getShaderName() const NOEXCEPT;

private:
  std::string createErrorMessage( const std::string& shaderName,
                                  SHADER_TYPE type,
                                  const std::string& error ) NOEXCEPT;

private:
  std::string shaderName{""};
  std::string errorMessage;
  std::string completeMessage{""};
  SHADER_TYPE type;
};

} /* namespace shaderProgram */
} /* namespace gl */
} /* namespace tetra */

#endif
