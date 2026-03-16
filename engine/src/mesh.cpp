#include "mesh.h"

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
} // namespace engine