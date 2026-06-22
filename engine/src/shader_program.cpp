#include "shader_program.h"

#include "glm/gtc/type_ptr.hpp"
#include "texture.h"

namespace engine
{

ShaderProgram::ShaderProgram(GLuint shaderProgramId) : shader_program_id_{shaderProgramId} {}

ShaderProgram::~ShaderProgram() { glDeleteProgram(shader_program_id_); }

void ShaderProgram::Bind()
{
    glUseProgram(shader_program_id_);
    current_texture_unit_ = 0;
}

GLint ShaderProgram::GetUniformLocation(const std::string& name)
{
    auto it = uniform_location_cache_.find(name);
    if (it != uniform_location_cache_.end())
    {
        return it->second;
    }

    auto location                 = glGetUniformLocation(shader_program_id_, name.c_str());
    uniform_location_cache_[name] = location;
    return location;
}

void ShaderProgram::SetUniform(const std::string& name, float value)
{
    auto location = GetUniformLocation(name);
    glUniform1f(location, value);
}

void ShaderProgram::SetUniform(const std::string& name, float v0, float v1)
{
    auto location = GetUniformLocation(name);
    glUniform2f(location, v0, v1);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& mat)
{
    auto location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::SetTexture(const std::string& name, Texture* texture)
{
    auto location = GetUniformLocation(name);
    glActiveTexture(GL_TEXTURE0 + current_texture_unit_);
    glBindTexture(GL_TEXTURE_2D, texture->Id());
    glUniform1i(location, current_texture_unit_);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& val)
{
    auto location = GetUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(val));
}

} // namespace engine