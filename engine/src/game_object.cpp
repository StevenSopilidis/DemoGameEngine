#include "game_object.h"

#include "animation_component.h"
#include "component.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "graphics_api.h"
#include "material.h"
#include "mesh.h"
#include "mesh_component.h"
#include "texture.h"
#include "vertex_layout.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#include "engine.h"

namespace
{

void ParseGLTFNode(cgltf_node* node, engine::GameObject* parent,
                   const std::filesystem::path& folder)
{
    auto obj = parent->GetScene()->CreateObject(node->name, parent);

    if (node->has_matrix)
    {
        auto      mat = glm::make_mat4(node->matrix);
        glm::vec3 translation, scale, skew;
        glm::vec4 perspective;
        glm::quat orentation;
        glm::decompose(mat, scale, orentation, translation, skew, perspective);

        obj->SetPosition(translation);
        obj->SetRotation(orentation);
        obj->SetScale(scale);
    }
    else
    {
        if (node->has_translation)
        {
            obj->SetPosition(
                glm::vec3(node->translation[0], node->translation[1], node->translation[2]));
        }

        if (node->has_rotation)
        {
            obj->SetRotation(glm::quat(node->rotation[3], node->rotation[0], node->rotation[1],
                                       node->rotation[2]));
        }

        if (node->has_scale)
        {
            obj->SetScale(glm::vec3(node->scale[0], node->scale[1], node->scale[2]));
        }
    }

    if (node->mesh)
    {
        for (cgltf_size pi{}; pi < node->mesh->primitives_count; pi++)
        {
            auto& primitive = node->mesh->primitives[pi];
            if (primitive.type != cgltf_primitive_type_triangles)
            {
                continue;
            }

            auto readFloats = [](const cgltf_accessor* acc, cgltf_size i, float* out, int n)
            {
                std::fill(out, out + n, 0.0f);
                return cgltf_accessor_read_float(acc, i, out, n) == 1;
            };

            auto readIndex = [](const cgltf_accessor* acc, cgltf_size i)
            {
                cgltf_uint out = 0;
                auto       ok  = cgltf_accessor_read_uint(acc, i, &out, 1);
                return ok ? static_cast<uint32_t>(out) : 0;
            };

            engine::VertexLayout           layout;
            std::array<cgltf_accessor*, 4> accessors{}; // pos, color, uvs, normals

            for (cgltf_size ai{0}; ai < primitive.attributes_count; ++ai)
            {
                auto& attr = primitive.attributes[ai];
                auto  acc  = attr.data;

                if (!acc)
                {
                    continue;
                }

                engine::VertexElement element;
                element.type = GL_FLOAT;

                switch (attr.type)
                {

                case cgltf_attribute_type_position:
                {
                    accessors[engine::VertexElement::PositionIndex] = acc;
                    element.index = engine::VertexElement::PositionIndex;
                    element.size  = 3;
                }
                break;
                case cgltf_attribute_type_color:
                {
                    if (attr.index != 0)
                    {
                        // only use first color channel
                        continue;
                    }
                    accessors[engine::VertexElement::ColorIndex] = acc;
                    element.index = engine::VertexElement::ColorIndex;
                    element.size  = 3;
                }
                break;
                case cgltf_attribute_type_texcoord:
                {
                    if (attr.index != 0)
                    {
                        // only use first text coord
                        continue;
                    }
                    accessors[engine::VertexElement::UVIndex] = acc;
                    element.index                             = engine::VertexElement::UVIndex;
                    element.size                              = 2;
                }
                break;
                case cgltf_attribute_type_normal:
                {
                    accessors[engine::VertexElement::NormalIndex] = acc;
                    element.index = engine::VertexElement::NormalIndex;
                    element.size  = 3;
                }
                break;

                default:
                    continue;
                }

                if (element.size > 0)
                {
                    element.offset = layout.stride;
                    layout.stride += element.size * sizeof(float);
                    layout.elements.push_back(element);
                }
            }

            if (accessors[engine::VertexElement::PositionIndex] == nullptr)
            {
                continue;
            }

            auto vertexCount = accessors[engine::VertexElement::PositionIndex]->count;

            std::vector<float> vertices;
            vertices.resize((layout.stride / sizeof(float)) * vertexCount);

            for (cgltf_size vi{0}; vi < vertexCount; ++vi)
            {
                for (auto& el : layout.elements)
                {
                    if (accessors[el.index] == nullptr)
                    {
                        continue;
                    }

                    auto  index   = (vi * layout.stride + el.offset) / sizeof(float);
                    auto* outData = &vertices[index];
                    readFloats(accessors[el.index], vi, outData, el.size);
                }
            }

            std::shared_ptr<engine::Mesh> mesh;

            if (primitive.indices != nullptr)
            {
                auto                  indexCount = primitive.indices->count;
                std::vector<uint32_t> indices(indexCount);

                for (cgltf_size i{0}; i < indexCount; ++i)
                {
                    indices[i] = readIndex(primitive.indices, i);
                }

                mesh = std::make_shared<engine::Mesh>(layout, vertices, indices);
            }
            else
            {
                mesh = std::make_shared<engine::Mesh>(layout, vertices);
            }

            auto mat = std::make_shared<engine::Material>();
            mat->SetShaderProgram(
                engine::Engine::GetInstance().GetGraphicsApi().GetDefaultSharedProgram());

            if (primitive.material)
            {
                auto* gltfMat = primitive.material;
                if (gltfMat->has_pbr_metallic_roughness)
                {
                    auto  pbr     = gltfMat->pbr_metallic_roughness;
                    auto* texture = pbr.base_color_texture.texture;

                    if (texture && texture->image)
                    {
                        if (texture->image->uri)
                        {
                            auto path = folder / std::string(texture->image->uri);
                            auto engineTexture =
                                engine::Engine::GetInstance().GetTextureManager().GetOrLoadTexture(
                                    path.string());
                            mat->SetParam("baseColorTexture", engineTexture);
                        }
                    }
                }
                else if (gltfMat->has_pbr_specular_glossiness)
                {
                    auto pbr     = gltfMat->pbr_specular_glossiness;
                    auto texture = pbr.diffuse_texture.texture;

                    if (texture && texture->image)
                    {
                        if (texture->image->uri)
                        {
                            auto path = folder / std::string(texture->image->uri);
                            auto engineTexture =
                                engine::Engine::GetInstance().GetTextureManager().GetOrLoadTexture(
                                    path.string());
                            mat->SetParam("baseColorTexture", engineTexture);
                        }
                    }
                }

                obj->AddComponent(new engine::MeshComponent(mat, mesh));
            }
        }
    }

    for (cgltf_size ci{}; ci < node->children_count; ci++)
    {
        ParseGLTFNode(node->children[ci], obj, folder);
    }
}

auto ReadScalar = [](cgltf_accessor* acc, cgltf_size index)
{
    float res{};
    cgltf_accessor_read_float(acc, index, &res, 1);
    return res;
};

auto ReadVec3 = [](cgltf_accessor* acc, cgltf_size index)
{
    glm::vec3 res;
    cgltf_accessor_read_float(acc, index, glm::value_ptr(res), 3);
    return res;
};

auto ReadQuat = [](cgltf_accessor* acc, cgltf_size index)
{
    std::array<float, 4> res = {0.0f, 0.0f, 0.0f, 1.0f};
    cgltf_accessor_read_float(acc, index, res.data(), 4);
    return glm::quat(res[3], res[0], res[1], res[2]);
};

auto ReadTimes = [](cgltf_accessor* acc, std::vector<float>& outTimes)
{
    outTimes.resize(acc->count);
    for (cgltf_size i{0}; i < acc->count; i++)
    {
        outTimes[i] = ReadScalar(acc, i);
    }
};

auto ReadOutputVec3 = [](cgltf_accessor* acc, std::vector<glm::vec3>& outValues)
{
    outValues.resize(acc->count);
    for (cgltf_size i{0}; i < acc->count; i++)
    {
        outValues[i] = ReadVec3(acc, i);
    }
};

auto ReadOutputQuat = [](cgltf_accessor* acc, std::vector<glm::quat>& outValues)
{
    outValues.resize(acc->count);
    for (cgltf_size i{0}; i < acc->count; i++)
    {
        outValues[i] = ReadQuat(acc, i);
    }
};

} // namespace

namespace engine
{
void GameObject::Update(float deltaTime)
{
    if (!is_active_)
    {
        return;
    }

    for (auto& component : components_)
    {
        component->Update(deltaTime);
    }

    auto result = std::ranges::remove_if(children_,
                                         [](const std::unique_ptr<GameObject>& child)
                                         {
                                             return !child->IsAlive(); // usually remove dead ones
                                         });

    children_.erase(result.begin(), result.end());

    for (auto& child : children_)
    {
        child->Update(deltaTime);
    }
}

const std::string& GameObject::Name() const { return name_; };

void GameObject::SetName(const std::string& name) { name_ = name; }

GameObject* GameObject::Parent() const { return parent_; }

bool GameObject::IsAlive() const { return is_alive_; }

void GameObject::MarkForDestroy() { is_alive_ = false; }

void GameObject::AddComponent(Component* component)
{
    components_.emplace_back(component);
    component->owner_ = this;
}

[[nodiscard]] const glm::vec3& GameObject::Position() const { return position_; }
[[nodiscard]] const glm::quat& GameObject::Rotation() const { return rotation_; }
[[nodiscard]] const glm::vec3& GameObject::Scale() const { return scale_; }

GameObject* GameObject::FindChildByName(const std::string& name)
{
    if (name_ == name)
    {
        return this;
    }

    for (auto& child : children_)
    {
        if (auto* result = child->FindChildByName(name))
        {
            return result;
        }
    }

    return nullptr;
}

void GameObject::SetPosition(glm::vec3 position) { position_ = position; }
void GameObject::SetRotation(glm::quat rotation) { rotation_ = rotation; }
void GameObject::SetScale(glm::vec3 scale) { scale_ = scale; };

[[nodiscard]] glm::mat4 GameObject::GetLocalTransform() const
{
    auto mat = glm::mat4(1.0f);
    // transformation
    mat = glm::translate(mat, position_);

    // rotation
    mat = mat * glm::mat4_cast(rotation_);

    // scaling
    mat = glm::scale(mat, scale_);

    return mat;
}

[[nodiscard]] glm::mat4 GameObject::GetWorldTransform() const
{
    if (parent_ != nullptr)
    {
        return parent_->GetWorldTransform() * GetLocalTransform();
    }

    return GetLocalTransform();
}

glm::vec3 GameObject::GetWorldPosition() const
{
    glm::vec4 hom = GetWorldTransform() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec3(hom) / hom.w;
}

GameObject* GameObject::LoadGLTF(const std::filesystem::path& path)
{
    auto content = Engine::GetInstance().GetFs().LoadAssetFile(path);

    if (content.empty())
    {
        return nullptr;
    }

    cgltf_options opts{};
    cgltf_data*   data{nullptr};

    cgltf_result res = cgltf_parse(&opts, content.data(), content.size(), &data);
    if (res != cgltf_result_success)
    {
        return nullptr;
    }

    auto fullPath           = Engine::GetInstance().GetFs().GetAssetsFolder() / path;
    auto fullFolderPath     = fullPath.remove_filename();
    auto relativeFolderPath = std::filesystem::path(path).remove_filename();

    res = cgltf_load_buffers(&opts, data, fullFolderPath.c_str());

    if (res != cgltf_result_success)
    {
        cgltf_free(data);
        return nullptr;
    }

    auto* resultObj = Engine::GetInstance().CurrentScene()->CreateObject("Result");
    auto* scene     = &data->scenes[0];

    for (cgltf_size i{}; i < scene->nodes_count; i++)
    {
        auto* node = scene->nodes[i];
        ParseGLTFNode(node, resultObj, relativeFolderPath);
    }

    std::vector<std::shared_ptr<AnimationClip>> clips;
    for (cgltf_size ai{}; ai < data->animations_count; ai++)
    {
        auto& anim = data->animations[ai];

        auto clip      = std::make_shared<AnimationClip>();
        clip->name     = anim.name ? anim.name : "noname";
        clip->duration = 0.0f;

        std::unordered_map<cgltf_node*, size_t> trackIndexOf;

        auto GetOrCreateTrack = [&](cgltf_node* node) -> TransformTrack&
        {
            auto it = trackIndexOf.find(node);
            if (it != trackIndexOf.end())
            {
                return clip->tracks[it->second];
            }

            TransformTrack track;
            track.target_name = node->name;
            clip->tracks.push_back(track);
            size_t idx         = clip->tracks.size() - 1;
            trackIndexOf[node] = idx;
            return clip->tracks[idx];
        };

        for (cgltf_size ci{}; ci < anim.channels_count; ci++)
        {
            auto& channel = anim.channels[ci];
            auto  sampler = channel.sampler;

            if (!channel.target_node || !sampler || !sampler->input || !sampler->output)
            {
                continue;
            }

            std::vector<float> times;
            ReadTimes(sampler->input, times);

            auto& track = GetOrCreateTrack(channel.target_node);

            switch (channel.target_path)
            {
            case cgltf_animation_path_type_translation:
            {
                std::vector<glm::vec3> values;
                ReadOutputVec3(sampler->output, values);
                track.positions.resize(times.size());
                for (size_t i{}; i < times.size(); i++)
                {
                    track.positions[i].time  = times[i];
                    track.positions[i].value = values[i];
                }
            }
            break;
            case cgltf_animation_path_type_rotation:
            {
                std::vector<glm::quat> values;
                ReadOutputQuat(sampler->output, values);
                track.rotations.resize(times.size());
                for (size_t i{}; i < times.size(); i++)
                {
                    track.rotations[i].time  = times[i];
                    track.rotations[i].value = values[i];
                }
            }
            break;
            case cgltf_animation_path_type_scale:
            {
                std::vector<glm::vec3> values;
                ReadOutputVec3(sampler->output, values);
                track.scales.resize(times.size());
                for (size_t i{}; i < times.size(); i++)
                {
                    track.scales[i].time  = times[i];
                    track.scales[i].value = values[i];
                }
            }
            break;
            default:
                break;
            }

            clip->duration = std::max(clip->duration, times.back());
        }

        clips.push_back(std::move(clip));
    }

    if (!clips.empty())
    {
        auto* animComponent = new AnimationComponent();
        resultObj->AddComponent(animComponent);

        for (auto& clip : clips)
        {
            animComponent->RegisterClip(clip->name, clip);
        }
    }

    cgltf_free(data);

    return resultObj;
}

void GameObject::SetActive(bool active) { is_active_ = active; }

bool GameObject::IsActive() const { return is_active_; }

bool GameObject::SetParent(GameObject* parent)
{
    if (!scene_)
    {
        return false;
    }

    return scene_->SetParent(this, parent);
}

Scene* GameObject::GetScene() { return scene_; }

} // namespace engine