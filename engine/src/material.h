#pragma once

#include <memory>
#include <unordered_map>
namespace engine
{
class ShaderProgram;

class Material
{
  public:
    void SetShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
    void SetParam(const std::string& name, float value);
    void SetParam(const std::string& name, float v0, float v1);
    void Bind();

  private:
    std::shared_ptr<ShaderProgram>                           shader_program_;
    std::unordered_map<std::string, float>                   float_params_;
    std::unordered_map<std::string, std::pair<float, float>> float2_params_;
};
} // namespace engine