#include "render_queue.h"

#include "graphics_api.h"
#include "material.h"
#include "mesh.h"

namespace engine
{
void RenderQueue::Submit(RenderCommand& command) { commands_.push_back(command); }

void RenderQueue::Draw(GraphicsApi& api)
{
    for (auto& command : commands_)
    {
        api.BindMaterial(command.material);
        api.BindMesh(command.mesh);
        api.DrawMesh(command.mesh);
    }

    commands_.clear();
}

} // namespace engine