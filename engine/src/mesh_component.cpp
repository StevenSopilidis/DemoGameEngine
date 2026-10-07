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
        auto&      matObj = json["material"];
        const auto path   = matObj.value("path", "");
        auto       mat    = Material::Load(path);
        if (mat && matObj.contains("params"))
        {
            auto& paramsObj = matObj["params"];

            // floats
            if (paramsObj.contains("float"))
            {
                for (auto& p : paramsObj["float"])
                {
                    std::string name  = p.value("name", "");
                    float       value = p.value("value", 0.0f);
                    mat->SetParam(name, value);
                }
            }

            // float2
            if (paramsObj.contains("float2"))
            {
                for (auto& p : paramsObj["float2"])
                {
                    std::string name   = p.value("name", "");
                    float       value0 = p.value("value0", 0.0f);
                    float       value1 = p.value("value1", 0.0f);
                    mat->SetParam(name, value0, value1);
                }
            }

            // float3
            if (paramsObj.contains("float3"))
            {
                for (auto& p : paramsObj["float3"])
                {
                    std::string name   = p.value("name", "");
                    float       value0 = p.value("value0", 0.0f);
                    float       value1 = p.value("value1", 0.0f);
                    float       value2 = p.value("value2", 0.0f);
                    mat->SetParam(name, glm::vec3(value0, value1, value2));
                }
            }

            // textures
            if (paramsObj.contains("textures"))
            {
                for (auto& p : paramsObj["textures"])
                {
                    std::string name = p.value("name", "");
                    std::string path = p.value("path", "");

                    auto texture = Texture::Load(path);
                    mat->SetParam(name, texture);
                }
            }
        }
        
        SetMaterial(mat);
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