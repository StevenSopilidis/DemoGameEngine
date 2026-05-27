#include "game.h"

#include "GLFW/glfw3.h"
#include "test_object.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Game::Init()
{
    auto& fs   = engine::Engine::GetInstance().GetFs();
    auto  path = fs.GetAssetsFolder() / "brick.png";

    int            width, height, channels;
    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    if (data)
    {
        std::cout << "Image loaded\n";
    }
    else
    {
        std::cout << "Image not loaded\n";
    }

    scene_       = new engine::Scene();
    auto* camera = scene_->CreateObject("Camera");
    camera->AddComponent(new engine::CameraComponent);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new engine::PlayerControllerComponent());

    scene_->SetMainCamera(camera);

    scene_->CreateObject<TestObject>("TestObject");

    engine::Engine::GetInstance().SetCurrentScene(scene_);

    return true;
}

void Game::Update(float deltaTime)
{
    auto& inputManager = engine::Engine::GetInstance().GetInputManager();

    scene_->Update(deltaTime);
}

void Game::Destroy() {}