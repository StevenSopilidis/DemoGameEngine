#include "shader_program.h"

namespace engine
{

ShaderProgram::ShaderProgram(GLuint shaderProgramId) : shader_program_id_{shaderProgramId} {}

ShaderProgram::~ShaderProgram() { glDeleteProgram(shader_program_id_); }

void ShaderProgram::Bind() { glUseProgram(shader_program_id_); }

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

} // namespace engine