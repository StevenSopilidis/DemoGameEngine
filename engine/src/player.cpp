#include "player.h"

#include "GLFW/glfw3.h"
#include "camera_component.h"
#include "player_controller_component.h"

#include <iostream>

void Player::Init()
{
    AddComponent(new engine::CameraComponent());
    SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    AddComponent(new engine::PlayerControllerComponent());

    auto* gunObject = engine::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    gunObject->SetParent(this);
    gunObject->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gunObject->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    if (auto* anim = gunObject->GetComponent<engine::AnimationComponent>())
    {
        if (auto* bullet = gunObject->FindChildByName("bullet_33"))
        {
            bullet->SetActive(true);
        }
        if (auto* file = gunObject->FindChildByName("BOOM_35"))
        {
            file->SetActive(true);
        }

        anim->Play("shoot");
    }
    else
    {
        std::cout << "Could not get animation\n";
        throw std::invalid_argument("Could not get animation");
    }

    animation_component_ = gunObject->GetComponent<engine::AnimationComponent>();
}

void Player::Update(float deltaTime)
{
    engine::GameObject::Update(deltaTime);

    auto& input = engine::Engine::GetInstance().GetInputManager();
    if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        if (animation_component_ != nullptr && !animation_component_->IsPlaying())
        {
            animation_component_->Play("shoot", false);
        }
    }
}
