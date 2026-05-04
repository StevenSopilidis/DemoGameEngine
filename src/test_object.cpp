#include "test_object.h"

#include "GLFW/glfw3.h"
#include "string"

TestObject::TestObject()
{
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform mat4 uModel;

        void main()
        {
            vColor = color;
            gl_Position = uModel* vec4(position, 1.0);
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

    material_.SetShaderProgram(shaderProgram);

    std::vector<float> vertices = {
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    };
    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    engine::VertexLayout layout;
    // define layout for position in vertex shader
    layout.elements.push_back({0, 3, GL_FLOAT, 0});
    // define layout for color in vertex shader
    layout.elements.push_back({1, 3, GL_FLOAT, sizeof(float) * 3});
    layout.stride = sizeof(float) * 6;

    mesh_ = std::make_unique<engine::Mesh>(layout, vertices, indices);
}

void TestObject::Update(float deltaTime)
{
    engine::GameObject::Update(deltaTime);

    auto  position = Position();
    auto& input    = engine::Engine::GetInstance().GetInputManager();

    if (input.IsKeyPressed(GLFW_KEY_A))
    {
        position.x -= 0.001f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_D))
    {
        position.x += 0.001f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_W))
    {
        position.y += 0.001f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_S))
    {
        position.y -= 0.001f;
    }
    SetPosition(position);

    engine::RenderCommand command;
    command.material     = &material_;
    command.mesh         = mesh_.get();
    command.model_matrix = GetWorldTransform();

    auto& queue = engine::Engine::GetInstance().GetRenderQueue();
    queue.Submit(command);
}