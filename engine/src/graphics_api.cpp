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

    auto fragmentShader     = glCreateShader(GL_FRAGMENT_SHADER);
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

    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
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

bool GraphicsApi::Init()
{
    glEnable(GL_DEPTH_TEST);
    return true;
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(),
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

void GraphicsApi::ClearBuffers() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

std::shared_ptr<ShaderProgram>& GraphicsApi::GetDefaultSharedProgram()
{
    if (!default_shader_program_)
    {
        std::string vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 color;
            layout (location = 2) in vec2 uv;
            layout (location = 3) in vec3 normal;
        
            out vec2 vUV;
            out vec3 vNormal;
            out vec3 vFragPos;
        
            uniform mat4 uModel;
            uniform mat4 uView;
            uniform mat4 uProjection;
        
            void main()
            {
                vUV = uv;
                vNormal = normalize(transpose(inverse(mat3(uModel))) * normal);
                vFragPos = vec3(uModel * vec4(position, 1.0));
                gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
            }
            )";

        std::string fragmentShaderSource = R"(
            #version 330 core

            struct Light
            {
                vec3 color;
                vec3 position;
            };

            uniform Light uLight;
            uniform vec3 uCameraPos;

            out vec4 FragColor;

            in vec2 vUV;
            in vec3 vNormal;
            in vec3 vFragPos;

            uniform sampler2D baseColorTexture;

            void main()
            {
                vec3 norm = normalize(vNormal);
                
                // diffuse
                vec3 lightDir = normalize(uLight.position - vFragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * uLight.color;

                // specular
                vec3 viewDir = normalize(uCameraPos - vFragPos);
                vec3 redlectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, redlectDir), 0.0), 32.0);
                float specularStrength = 0.5;
                vec3 specular = specularStrength * spec * uLight.color;
                
                vec3 result = diffuse + specular;

                vec4 texColor = texture(baseColorTexture, vUV);

                FragColor = texColor * vec4(result, 1.0);
            }
            )";

        default_shader_program_ = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    }

    return default_shader_program_;
}

} // namespace engine