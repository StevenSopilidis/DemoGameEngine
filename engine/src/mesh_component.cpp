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

    auto& queue = engine::GetInstance().GetRenderQueue();
    queue.Submit(command);
}

} // namespace engine