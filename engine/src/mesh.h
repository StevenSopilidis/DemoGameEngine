#pragma once

#include "GL/glew.h"
#include "vertex_layout.h"

#include <filesystem>
#include <memory>

namespace engine
{
class Mesh
{
  public:
    Mesh(VertexLayout& layout, const std::vector<float>& vertices,
         const std::vector<uint32_t>& indices);
    Mesh(VertexLayout& layout, const std::vector<float>& vertices);

    Mesh(const Mesh&)            = default;
    Mesh& operator=(const Mesh&) = default;

    void Bind();
    void Draw();

    static std::shared_ptr<Mesh> Load(const std::filesystem::path& path);

  private:
    VertexLayout vertex_layout_;
    std::size_t  vertex_count_{0};
    std::size_t  index_count_{0};

    GLuint vbo_{0};
    GLuint ebo_{0};
    GLuint vao_{0};
};
} // namespace engine