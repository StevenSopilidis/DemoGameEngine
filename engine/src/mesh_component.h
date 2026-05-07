#pragma once

#include "component.h"

#include <memory>

namespace engine
{
class Material;
class Mesh;

class MeshComponent : public Component
{
  public:
    MeshComponent(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh);
    void Update(float deltaTime) override;

  private:
    std::shared_ptr<Material> material_;
    std::shared_ptr<Mesh>     mesh_;
};
} // namespace engine