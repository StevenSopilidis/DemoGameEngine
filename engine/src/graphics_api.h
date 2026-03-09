#pragma once

#include "GL/glew.h"

#include <memory>

namespace engine
{
class ShaderProgram;

class GraphicsApi
{
  public:
    std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexSource,
                                                       const std::string& fragmentSource);

    void BindShaderProgram(ShaderProgram* program);
};
} // namespace engine