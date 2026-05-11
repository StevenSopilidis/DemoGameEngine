#include "game.h"

#include "test_object.h"

bool Game::Init()
{
    scene_       = new engine::Scene();
    auto* camera = scene_->CreateObject("Camera");
    camera->AddComponent(new engine::CameraComponent);
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

    scene_->SetMainCamera(camera);

    scene_->CreateObject<TestObject>("TestObject");

    engine::Engine::GetInstance().SetCurrentScene(scene_);

    return true;
}

void Game::Update(float deltaTime) { scene_->Update(deltaTime); }

void Game::Destroy() {}