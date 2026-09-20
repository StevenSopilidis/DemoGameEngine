#pragma once

#include "component.h"
#include "nlohmann/json.hpp"

#include <memory>

namespace engine
{
class Material;
class Mesh;

class MeshComponent : public Component
{
    COMPONENT(MeshComponent)

  public:
    MeshComponent() = default;
    MeshComponent(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh);
    void Update(float deltaTime) override;
    void LoadProperties(const nlohmann::json& json) override;
    void SetMaterial(const std::shared_ptr<Material>& material);
    void SetMesh(const std::shared_ptr<Mesh>& mesh);

  private:
    std::shared_ptr<Material> material_;
    std::shared_ptr<Mesh>     mesh_;
};
} // namespace engine