#include "graphics_api.h"

#include "material.h"
#include "mesh.h"
#include "shader_program.h"

namespace engine
{

std::shared_ptr<ShaderProgram> GraphicsApi::CreateShaderProgram(const std::string& vertexSource,
                                                                const std::string& fragmentSource)
{
    auto vertexShader     = glCreateShader(GL_VERTEX_SHADER);
    auto vertexShaderCStr = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::array<char, 512> infoLog;
        glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
        return nullptr;
    }

    auto fragmentShader     = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShaderCStr = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderCStr, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::array<char, 512> infoLog;
        glGetShaderInfoLog(fragmentShader, infoLog.size(), nullptr, infoLog.data());
        return nullptr;
    }

    auto shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShader);
    glAttachShader(shaderProgramId, fragmentShader);
    glLinkProgram(shaderProgramId);

    if (!success)
    {
        std::array<char, 512> infoLog;
        glGetProgramInfoLog(shaderProgramId, infoLog.size(), nullptr, infoLog.data());
        return nullptr;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return std::make_shared<ShaderProgram>(shaderProgramId);
}

void GraphicsApi::BindShaderProgram(ShaderProgram& program) { program.Bind(); }

void GraphicsApi::BindMaterial(Material* material) { material->Bind(); }

GLuint GraphicsApi::CreateVertexBuffer(const std::vector<float>& vertices)
{
    GLuint vbo{0};
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

GLuint GraphicsApi::CreateIndexBuffer(const std::vector<uint32_t>& indices)
{
    GLuint ebo{0};
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return ebo;
}

void GraphicsApi::BindMesh(Mesh* mesh)
{
    if (mesh != nullptr)
    {
        mesh->Bind();
    }
}

void GraphicsApi::DrawMesh(Mesh* mesh)
{
    if (mesh != nullptr)
    {
        mesh->Draw();
    }
}

void GraphicsApi::SetClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }

void GraphicsApi::ClearBuffers() { glClear(GL_COLOR_BUFFER_BIT); }

} // namespace engine