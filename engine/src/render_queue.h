#pragma once

#include "glm/ext/matrix_float4x4.hpp"

#include <vector>

namespace engine
{
class Mesh;
class Material;
class GraphicsApi;

struct RenderCommand
{
    Mesh*     mesh{nullptr};
    Material* material{nullptr};
    glm::mat4 model_matrix;
};

struct CameraData
{
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

class RenderQueue
{
  public:
    void Submit(RenderCommand& command);
    void Draw(GraphicsApi& api, const CameraData& cameraData);

  private:
    std::vector<RenderCommand> commands_;
};
} // namespace engine