#pragma once

#include "GL/glew.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "texture.h"

#include <string>
#include <unordered_map>

namespace engine
{
class Texture;

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
    void  SetUniform(const std::string& name, float v0, float v1);
    void  SetUniform(const std::string& name, const glm::mat4& mat);
    void  SetTexture(const std::string& name, Texture* texture);

  private:
    std::unordered_map<std::string, GLint> uniform_location_cache_;
    GLuint                                 shader_program_id_{0};
    int                                    current_texture_unit_{0};
};
} // namespace engine