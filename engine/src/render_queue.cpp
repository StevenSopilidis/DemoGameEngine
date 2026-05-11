#include "render_queue.h"

#include "graphics_api.h"
#include "material.h"
#include "mesh.h"
#include "shader_program.h"

namespace engine
{
void RenderQueue::Submit(RenderCommand& command) { commands_.push_back(command); }

void RenderQueue::Draw(GraphicsApi& api, const CameraData& cameraData)
{
    for (auto& command : commands_)
    {
        api.BindMaterial(command.material);
        auto shaderProgram = command.material->GetShaderProgram();
        shaderProgram->SetUniform("uModel", command.model_matrix);

        shaderProgram->SetUniform("uView", cameraData.viewMatrix);
        shaderProgram->SetUniform("uProjection", cameraData.projectionMatrix);

        api.BindMesh(command.mesh);
        api.DrawMesh(command.mesh);
    }

    commands_.clear();
}

} // namespace engine