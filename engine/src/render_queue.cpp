#include "render_queue.h"

#include "graphics_api.h"
#include "material.h"
#include "mesh.h"
#include "shader_program.h"

namespace engine
{
void RenderQueue::Submit(RenderCommand& command) { commands_.push_back(command); }

void RenderQueue::Draw(GraphicsApi& api)
{
    for (auto& command : commands_)
    {
        api.BindMaterial(command.material);
        command.material->GetShaderProgram()->SetUniform("uModel", command.model_matrix);
        api.BindMesh(command.mesh);
        api.DrawMesh(command.mesh);
    }

    commands_.clear();
}

} // namespace engine