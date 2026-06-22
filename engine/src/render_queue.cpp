#include "render_queue.h"

#include "graphics_api.h"
#include "material.h"
#include "mesh.h"
#include "shader_program.h"

namespace engine
{
void RenderQueue::Submit(RenderCommand& command) { commands_.push_back(command); }

void RenderQueue::Draw(GraphicsApi& api, const CameraData& cameraData,
                       const std::vector<LightData>& lights)
{
    for (auto& command : commands_)
    {
        api.BindMaterial(command.material);
        auto shaderProgram = command.material->GetShaderProgram();
        shaderProgram->SetUniform("uModel", command.model_matrix);

        shaderProgram->SetUniform("uView", cameraData.viewMatrix);
        shaderProgram->SetUniform("uProjection", cameraData.projectionMatrix);

        if (!lights.empty())
        {
            auto& light = lights.at(0);
            shaderProgram->SetUniform("uLight.color", light.color);
            shaderProgram->SetUniform("uLight.position", light.pos);
        }

        api.BindMesh(command.mesh);
        api.DrawMesh(command.mesh);
    }

    commands_.clear();
}

} // namespace engine