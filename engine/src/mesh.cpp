#include "mesh.h"

#include "engine.h"

#include <array>

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

std::shared_ptr<Mesh> Mesh::CreateBox(const glm::vec3& extents)
{
    const glm::vec3    half     = extents * 0.5f;
    std::vector<float> vertices = {
        // Front face
        half.x, half.y, half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -half.x, half.y,
        half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -half.x, -half.y, half.z, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, half.x, -half.y, half.z, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f,

        // Top face
        half.x, half.y, -half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -half.x, half.y,
        -half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -half.x, half.y, half.z, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, half.x, half.y, half.z, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f,

        // Right face
        half.x, half.y, -half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, half.x, half.y,
        half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, half.x, -half.y, half.z, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, half.x, -half.y, -half.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Left face
        -half.x, half.y, half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -half.x, half.y,
        -half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, -half.x, -half.y, -half.z, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -half.x, -half.y, half.z, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, -1.0f, 0.0f, 0.0f,

        // Bottom face
        half.x, -half.y, half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -half.x, -half.y,
        half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -half.x, -half.y, -half.z, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, half.x, -half.y, -half.z, 1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 0.0f,

        // Back face
        -half.x, half.y, -half.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, half.x, half.y,
        -half.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, half.x, -half.y, -half.z, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -half.x, -half.y, -half.z, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f};

    std::vector<unsigned int> indices = {// front face
                                         0, 1, 2, 0, 2, 3,
                                         // top face
                                         4, 5, 6, 4, 6, 7,
                                         // right face
                                         8, 9, 10, 8, 10, 11,
                                         // left face
                                         12, 13, 14, 12, 14, 15,
                                         // bottom face
                                         16, 17, 18, 16, 18, 19,
                                         // back face
                                         20, 21, 22, 20, 22, 23};

    engine::VertexLayout vertexLayout;

    // Postion
    vertexLayout.elements.push_back({VertexElement::PositionIndex, 3, GL_FLOAT, 0});
    // Color
    vertexLayout.elements.push_back({VertexElement::ColorIndex, 3, GL_FLOAT, sizeof(float) * 3});
    // UV
    vertexLayout.elements.push_back({VertexElement::UVIndex, 2, GL_FLOAT, sizeof(float) * 6});
    // Normal
    vertexLayout.elements.push_back({VertexElement::NormalIndex, 3, GL_FLOAT, sizeof(float) * 8});
    vertexLayout.stride = sizeof(float) * 11;

    auto result = std::make_shared<engine::Mesh>(vertexLayout, vertices, indices);

    return result;
}

} // namespace engine