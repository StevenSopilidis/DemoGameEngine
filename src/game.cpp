#include "game.h"

#include "GLFW/glfw3.h"
#include "light_component.h"
#include "test_object.h"

#include <iostream>

bool Game::Init()
{
    auto& fs      = engine::Engine::GetInstance().GetFs();
    auto  texture = engine::Texture::Load("textures/brick.png");
    if (!texture)
    {
        std::cout << "Failed to load brick.png\n";
        return false;
    }

    scene_ = new engine::Scene();
    engine::Engine::GetInstance().SetCurrentScene(scene_);

    auto* camera = scene_->CreateObject("Camera");
    camera->AddComponent(new engine::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new engine::PlayerControllerComponent());

    scene_->SetMainCamera(camera);

    scene_->CreateObject<TestObject>("TestObject");

    auto* suzanneObject = engine::GameObject::LoadGLTF("models/suzanne/Suzanne.gltf");
    suzanneObject->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

    auto* gunObject = engine::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    gunObject->SetParent(camera);
    gunObject->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gunObject->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    if (auto anim = gunObject->GetComponent<engine::AnimationComponent>())
    {
        if (auto bullet = gunObject->FindChildByName("bullet_33"))
        {
            bullet->SetActive(true);
        }
        if (auto file = gunObject->FindChildByName("BOOM_35"))
        {
            file->SetActive(true);
        }

        anim->Play("shoot");
    }
    else
    {
        std::cout << "Could not get animation\n";
    }

    auto* light     = scene_->CreateObject("Light");
    auto* lightComp = new engine::LightComponent();
    lightComp->SetColor(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    return true;
}

void Game::Update(float deltaTime)
{
    auto& inputManager = engine::Engine::GetInstance().GetInputManager();

    scene_->Update(deltaTime);
}

void Game::Destroy() {}