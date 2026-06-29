#include "game.h"

#include "GLFW/glfw3.h"
#include "light_component.h"
#include "test_object.h"

#include <iostream>

bool Game::Init()
{
    scene_       = new engine::Scene();
    auto* camera = scene_->CreateObject("Camera");
    camera->AddComponent(new engine::CameraComponent);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new engine::PlayerControllerComponent());

    scene_->SetMainCamera(camera);

    scene_->CreateObject<TestObject>("TestObject");

    auto light     = scene_->CreateObject("Light");
    auto lightComp = new engine::LightComponent();
    lightComp->SetColor(glm::vec3(1.0f));
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    light->AddComponent(lightComp);

    engine::Engine::GetInstance().SetCurrentScene(scene_);

    return true;
}

void Game::Update(float deltaTime)
{
    auto& inputManager = engine::Engine::GetInstance().GetInputManager();

    scene_->Update(deltaTime);
}

void Game::Destroy() {}