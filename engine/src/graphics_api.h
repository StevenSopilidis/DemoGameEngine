#pragma once

#include "GL/glew.h"

#include <memory>
#include <vector>

namespace engine
{
class ShaderProgram;
class Material;
class Mesh;

class GraphicsApi
{
  public:
    std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexSource,
                                                       const std::string& fragmentSource);

    void   BindShaderProgram(ShaderProgram& program);
    void   BindMaterial(Material* material);
    GLuint CreateVertexBuffer(const std::vector<float>& vertices);
    GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);
    void   BindMesh(Mesh* mesh);
    void   DrawMesh(Mesh* mesh);
    void   SetClearColor(float r, float g, float b, float a);
    void   ClearBuffers();
};
} // namespace engine