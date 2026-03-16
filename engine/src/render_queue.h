#pragma once

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
};

class RenderQueue
{
  public:
    void Submit(RenderCommand& command);
    void Draw(GraphicsApi& api);

  private:
    std::vector<RenderCommand> commands_;
};
} // namespace engine