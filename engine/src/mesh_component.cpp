#include "mesh_component.h"

#include "engine.h"
#include "game_object.h"
#include "material.h"
#include "mesh.h"
#include "render_queue.h"

namespace engine
{
MeshComponent::MeshComponent(const std::shared_ptr<Material>& material,
                             const std::shared_ptr<Mesh>&     mesh)
    : material_{material}, mesh_{mesh}
{
}

void MeshComponent::LoadProperties(const nlohmann::json& json)
{
    if (json.contains("material"))
    {
        const auto matPath  = json.value("material", "");
        auto       material = Material::Load(matPath);
        if (material)
        {
            SetMaterial(material);
        }
    }

    if (json.contains("mesh"))
    {
        const auto&       meshObj = json["mesh"];
        const std::string type    = meshObj.value("type", "box");

        if (type == "box")
        {
            glm::vec3 extents(meshObj.value("x", 1.0f), meshObj.value("y", 1.0f),
                              meshObj.value("z", 1.0f));

            auto mesh = Mesh::CreateBox(extents);
            SetMesh(mesh);
        }
    }
}

void MeshComponent::Update(float deltaTime)
{
    if (!material_ || !mesh_)
    {
        return;
    }

    engine::RenderCommand command;
    command.material     = material_.get();
    command.mesh         = mesh_.get();
    command.model_matrix = Owner()->GetWorldTransform();

    auto& queue = Engine::GetInstance().GetRenderQueue();
    queue.Submit(command);
}

void MeshComponent::SetMaterial(const std::shared_ptr<Material>& material) { material_ = material; }

void MeshComponent::SetMesh(const std::shared_ptr<Mesh>& mesh) { mesh_ = mesh; }

} // namespace engine