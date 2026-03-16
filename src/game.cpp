#include "game.h"

#include "GLFW/glfw3.h"
#include "string"

#include <iostream>

bool Game::Init()
{
    std::string vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        out vec3 vColor;

        uniform vec2 uOffset;

        void main()
        {
            vColor = color;
            gl_Position = vec4(position.x + uOffset.x, position.y + uOffset.y, position.z, 1.0);
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

    return shaderProgram != nullptr;
}

void Game::Update(float deltaTime)
{
    auto& input = engine::Engine::GetInstance().GetInputManager();

    if (input.IsKeyPressed(GLFW_KEY_A))
    {
        offset_x_ -= 0.001f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_D))
    {
        offset_x_ += 0.001f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_W))
    {
        offset_y_ += 0.001f;
    }
    else if (input.IsKeyPressed(GLFW_KEY_S))
    {
        offset_y_ -= 0.001f;
    }

    material_.SetParam("uOffset", offset_x_, offset_y_);

    engine::RenderCommand command;
    command.material = &material_;
    command.mesh     = mesh_.get();

    auto& queue = engine::Engine::GetInstance().GetRenderQueue();
    queue.Submit(command);
}

void Game::Destroy() {}