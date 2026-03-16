#include "material.h"

#include "shader_program.h"

namespace engine
{
void Material::SetShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram)
{
    shader_program_ = shaderProgram;
}

void Material::SetParam(const std::string& name, float value) { float_params_[name] = value; }

void Material::SetParam(const std::string& name, float v0, float v1)
{
    float2_params_[name] = {v0, v1};
}

void Material::Bind()
{
    if (!shader_program_)
    {
        return;
    }

    shader_program_->Bind();

    for (const auto& param : float_params_)
    {
        shader_program_->SetUniform(param.first, param.second);
    }

    for (const auto& param : float2_params_)
    {
        shader_program_->SetUniform(param.first, param.second.first, param.second.second);
    }
}

} // namespace engine