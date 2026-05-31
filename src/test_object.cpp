#include "test_object.h"

#include "GLFW/glfw3.h"
#include "mesh_component.h"
#include "string"

TestObject::TestObject()
{
    std::string vertexShaderSource   = engine::Fs::LoadAssetFileText("shaders/vertex.glsl");
    std::string fragmentShaderSource = engine::Fs::LoadAssetFileText("shaders/fragment.glsl");

    auto& graphicsAPI   = engine::Engine::GetInstance().GetGraphicsApi();
    auto  shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    auto texture = engine::Texture::Load("brick.png");

    auto material = std::make_shared<engine::Material>();
    material->SetShaderProgram(shaderProgram);
    material->SetParam("BrickTexture", texture);

    std::vector<float> vertices = {
        // front face
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,

        // top face
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        -0.5f,
        0.5f,
        0.5f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.5f,
        0.5f,
        0.5f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,

        // right face
        0.5f,
        0.5f,
        -0.5f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        0.5f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,

        // left face
        -0.5f,
        0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        -0.5f,
        -0.5f,
        0.5f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,

        // bottom face
        0.5f,
        -0.5f,
        0.5f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -0.5f,
        -0.5f,
        0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,

        // back face
        -0.5f,
        0.5f,
        -0.5f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.5f,
        0.5f,
        -0.5f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.5f,
        -0.5f,
        -0.5f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        -0.5f,
        -0.5f,
        -0.5f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
    };

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

    engine::VertexLayout layout;
    // define layout for position in vertex shader
    layout.elements.push_back({0, 3, GL_FLOAT, 0});
    // define layout for color in vertex shader
    layout.elements.push_back({1, 3, GL_FLOAT, sizeof(float) * 3});
    // define layout for uv in vertex shader
    layout.elements.push_back({2, 2, GL_FLOAT, sizeof(float) * 6});

    layout.stride = sizeof(float) * 8;

    auto mesh = std::make_shared<engine::Mesh>(layout, vertices, indices);

    AddComponent(new engine::MeshComponent(material, mesh));
}

void TestObject::Update(float deltaTime) { engine::GameObject::Update(deltaTime); }