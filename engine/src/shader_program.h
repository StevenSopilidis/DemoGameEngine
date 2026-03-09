#pragma once

#include "GL/glew.h"

#include <string>
#include <unordered_map>

namespace engine
{
class ShaderProgram
{
  public:
    ShaderProgram(const ShaderProgram&)            = delete;
    ShaderProgram(ShaderProgram&&)                 = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram& operator=(ShaderProgram&&)      = delete;

    explicit ShaderProgram(GLuint shaderProgramId);
    ShaderProgram() = delete;
    ~ShaderProgram();

    void  Bind();
    GLint GetUniformLocation(const std::string& name);
    void  SetUniform(const std::string& name, float value);

  private:
    std::unordered_map<std::string, GLint> uniform_location_cache_;
    GLuint                                 shader_program_id_{0};
};
} // namespace engine