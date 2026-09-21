#include "player.h"

#include "GLFW/glfw3.h"
#include "camera_component.h"
#include "player_controller_component.h"

#include <iostream>

void Player::Init()
{
    if (auto* bullet = FindChildByName("bullet_33"))
    {
        bullet->SetActive(false);
    }

    if (auto* fire = FindChildByName("BOOM_35"))
    {
        fire->SetActive(false);
    }

    if (auto* gun = FindChildByName("Gun"))
    {
        animation_component_ = gun->GetComponent<engine::AnimationComponent>();
    }
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
