#include "test_object.h"

#include "GLFW/glfw3.h"
#include "mesh_component.h"
#include "string"

TestObject::TestObject()
{
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;


        void main()
        {
            vColor = color;
            gl_Position = uProjection * uView *  uModel* vec4(position, 1.0);
        }
    )";

    std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 vColor;

        void main()
        {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    auto& graphicsAPI   = engine::Engine::GetInstance().GetGraphicsApi();
    auto  shaderProgram = graphicsAPI.CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    auto material = std::make_shared<engine::Material>();
    material->SetShaderProgram(shaderProgram);

    std::vector<float> vertices = {
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
        0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    };
    std::vector<unsigned int> indices = 
    {
        // front face
        0, 1, 2,
        0, 2, 3,
        // top face
        4, 5, 1,
        4, 1, 0,
        // right face
        4, 0, 3,
        4, 3, 7,
        // left face
        1, 5, 6,
        1, 6, 2,
        // bottom face
        3, 2, 6,
        3, 6, 7,
        // back face
        4, 7, 6,
        4, 6, 5
    };

    engine::VertexLayout layout;
    // define layout for position in vertex shader
    layout.elements.push_back({0, 3, GL_FLOAT, 0});
    // define layout for color in vertex shader
    layout.elements.push_back({1, 3, GL_FLOAT, sizeof(float) * 3});
    layout.stride = sizeof(float) * 6;

    auto mesh = std::make_shared<engine::Mesh>(layout, vertices, indices);

    AddComponent(new engine::MeshComponent(material, mesh));
}

void TestObject::Update(float deltaTime) { engine::GameObject::Update(deltaTime); }