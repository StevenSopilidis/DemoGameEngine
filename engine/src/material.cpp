#include "material.h"

#include "engine.h"
#include "nlohmann/json.hpp"
#include "shader_program.h"
#include "texture.h"

namespace engine
{
void Material::SetShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram)
{
    shader_program_ = shaderProgram;
}

std::shared_ptr<ShaderProgram> Material::GetShaderProgram() { return shader_program_; }

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

    for (const auto& param : textures_)
    {
        shader_program_->SetTexture(param.first, param.second.get());
    }
}

void Material::SetParam(const std::string& name, const std::shared_ptr<Texture>& texture)
{
    textures_[name] = texture;
}

std::shared_ptr<Material> Material::Load(const std::filesystem::path& path)
{
    auto content = Engine::GetInstance().GetFs().LoadAssetFileText(path);

    if (content.empty())
    {
        return nullptr;
    }

    nlohmann::json json = nlohmann::json::parse(content);

    auto result = std::make_shared<Material>();

    if (json.contains("shader"))
    {
        auto shaderObj     = json["shader"];
        auto vertexPath    = shaderObj.value("vertex", "");
        auto fragmentxPath = shaderObj.value("fragment", "");

        auto vertexSrc   = Engine::GetInstance().GetFs().LoadAssetFileText(vertexPath);
        auto fragmentSrc = Engine::GetInstance().GetFs().LoadAssetFileText(fragmentxPath);

        auto shaderProgram =
            Engine::GetInstance().GetGraphicsApi().CreateShaderProgram(vertexSrc, fragmentSrc);

        if (!shaderProgram)
        {
            return nullptr;
        }

        result->SetShaderProgram(shaderProgram);
    }

    if (json.contains("params"))
    {
        auto paramsObj = json["params"];

        // floats
        if (paramsObj.contains("float"))
        {
            for (auto& p : paramsObj["float"])
            {
                std::string name  = p.value("name", "");
                float       value = p.value("value", 0.0f);
                result->SetParam(name, value);
            }
        }

        // float2
        if (paramsObj.contains("float2"))
        {
            for (auto& p : paramsObj["float2"])
            {
                std::string name   = p.value("name", "");
                float       value0 = p.value("value0", 0.0f);
                float       value1 = p.value("value1", 0.0f);
                result->SetParam(name, value0, value1);
            }
        }

        // textures
        if (paramsObj.contains("textures"))
        {
            for (auto& p : paramsObj["textures"])
            {
                std::string name = p.value("name", "");
                std::string path = p.value("path", "");

                auto texture = Texture::Load(path);
                result->SetParam(name, texture);
            }
        }
    }

    return result;
}

} // namespace engine