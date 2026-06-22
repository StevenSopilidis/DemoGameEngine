#include "mesh.h"

#include <array>

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#include "engine.h"

namespace engine
{
Mesh::Mesh(VertexLayout& layout, const std::vector<float>& vertices,
           const std::vector<uint32_t>& indices)
    : vertex_layout_{std::move(layout)}
{
    auto& api = Engine::GetInstance().GetGraphicsApi();

    vbo_ = api.CreateVertexBuffer(vertices);
    ebo_ = api.CreateIndexBuffer(indices);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    for (auto& element : vertex_layout_.elements)
    {
        glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE,
                              vertex_layout_.stride, (void*)(uintptr_t)element.offset);

        glEnableVertexAttribArray(element.index);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    vertex_count_ = (vertices.size() * sizeof(float)) / vertex_layout_.stride;
    index_count_  = indices.size();
}

Mesh::Mesh(VertexLayout& layout, const std::vector<float>& vertices)
    : vertex_layout_{std::move(layout)}

{
    auto& api = Engine::GetInstance().GetGraphicsApi();

    vbo_ = api.CreateVertexBuffer(vertices);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    for (auto& element : vertex_layout_.elements)
    {
        glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE,
                              vertex_layout_.stride, (void*)(uintptr_t)element.offset);

        glEnableVertexAttribArray(element.index);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Bind() { glBindVertexArray(vao_); }

void Mesh::Draw()
{
    if (index_count_ > 0)
    {
        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
    }
}

std::shared_ptr<Mesh> Mesh::Load(const std::filesystem::path& path)
{
    auto content = Engine::GetInstance().GetFs().LoadAssetFileText(path);

    if (content.empty())
    {
        return nullptr;
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

    cgltf_options opts{};
    cgltf_data*   data{nullptr};

    cgltf_result res = cgltf_parse(&opts, content.data(), content.size(), &data);
    if (res != cgltf_result_success)
    {
        return nullptr;
    }

    auto fullPath = Engine::GetInstance().GetFs().GetAssetsFolder() / path;
    res           = cgltf_load_buffers(&opts, data, fullPath.remove_filename().c_str());

    if (res != cgltf_result_success)
    {
        cgltf_free(data);
        return nullptr;
    }

    std::shared_ptr<Mesh> result = {nullptr};
    for (cgltf_size mesh_index{0}; mesh_index < data->meshes_count; ++mesh_index)
    {
        auto mesh = data->meshes[mesh_index];
        for (cgltf_size pi = 0; pi < mesh.primitives_count; ++pi)
        {
            auto& primitive = mesh.primitives[pi];
            if (primitive.type != cgltf_primitive_type_triangles)
            {
                continue;
            }

            VertexLayout                   layout;
            std::array<cgltf_accessor*, 4> accessors{}; // pos, color, uvs, normals

            for (cgltf_size ai{0}; ai < primitive.attributes_count; ++ai)
            {
                auto& attr = primitive.attributes[ai];
                auto  acc  = attr.data;

                if (!acc)
                {
                    continue;
                }

                VertexElement element;
                element.type = GL_FLOAT;

                switch (attr.type)
                {

                case cgltf_attribute_type_position:
                {
                    accessors[VertexElement::PositionIndex] = acc;
                    element.index                           = VertexElement::PositionIndex;
                    element.size                            = 3;
                }
                break;
                case cgltf_attribute_type_color:
                {
                    if (attr.index != 0)
                    {
                        // only use first color channel
                        continue;
                    }
                    accessors[VertexElement::ColorIndex] = acc;
                    element.index                        = VertexElement::ColorIndex;
                    element.size                         = 3;
                }
                break;
                case cgltf_attribute_type_texcoord:
                {
                    if (attr.index != 0)
                    {
                        // only use first text coord
                        continue;
                    }
                    accessors[VertexElement::UVIndex] = acc;
                    element.index                     = VertexElement::UVIndex;
                    element.size                      = 2;
                }
                break;
                case cgltf_attribute_type_normal:
                {
                    accessors[VertexElement::NormalIndex] = acc;
                    element.index                         = VertexElement::NormalIndex;
                    element.size                          = 3;
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

            if (accessors[VertexElement::PositionIndex] == nullptr)
            {
                continue;
            }

            auto vertexCount = accessors[VertexElement::PositionIndex]->count;

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

            if (primitive.indices != nullptr)
            {
                auto                  indexCount = primitive.indices->count;
                std::vector<uint32_t> indices(indexCount);

                for (cgltf_size i{0}; i < indexCount; ++i)
                {
                    indices[i] = readIndex(primitive.indices, i);
                }

                result = std::make_shared<Mesh>(layout, vertices, indices);
            }
            else
            {
                result = std::make_shared<Mesh>(layout, vertices);
            }

            if (result)
            {
                break;
            }
        }

        if (result)
        {
            break;
        }
    }

    cgltf_free(data);

    return result;
}

} // namespace engine