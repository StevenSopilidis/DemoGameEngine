#include "player.h"

#include "GLFW/glfw3.h"
#include "bullet.h"
#include "camera_component.h"
#include "physics_component.h"
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

    audio_component_             = GetComponent<engine::AudioComponent>();
    player_controller_component_ = GetComponent<engine::PlayerControllerComponent>();
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

        if (audio_component_ != nullptr)
        {
            if (audio_component_->IsPlaying("shoot"))
            {
                audio_component_->Stop("shoot");
            }

            audio_component_->Play("shoot");
        }

        auto* bullet         = scene_->CreateObject<Bullet>("bullet");
        auto  bulletMaterial = engine::Material::Load("materials/suzanne.mat.json");
        auto  bulletMesh     = engine::Mesh::CreateSphere(0.2f, 32, 32);
        bullet->AddComponent(new engine::MeshComponent(bulletMaterial, bulletMesh));

        auto pos = glm::vec3(0.0f);
        if (auto* child = FindChildByName("BOOM_35"))
        {
            pos = child->GetWorldPosition();
        }
        bullet->SetPosition(pos + rotation_ * glm::vec3(-0.2f, 0.2f, -1.75f));

        auto collider = std::make_shared<engine::SphereCollider>(0.2f);
        auto rigidBody =
            std::make_shared<engine::RigidBody>(engine::BodyType::Dynamic, collider, 10, 0.1f);

        bullet->AddComponent(new engine::PhysicsComponent(rigidBody));

        auto frontVector = rotation_ * glm::vec3(0.0f, 0.0f, -1.0f);
        rigidBody->ApplyImpulse(frontVector * 500.0f);
    }

    if (input.IsKeyPressed(GLFW_KEY_SPACE))
    {
        if (audio_component_ != nullptr && !audio_component_->IsPlaying("jump"))
        {
            audio_component_->Play("jump");
        }
    }

    bool walking = input.IsKeyPressed(GLFW_KEY_W) || input.IsKeyPressed(GLFW_KEY_A) ||
                   input.IsKeyPressed(GLFW_KEY_D) || input.IsKeyPressed(GLFW_KEY_S);

    if (walking && player_controller_component_ != nullptr &&
        player_controller_component_->OnGround())
    {
        if (audio_component_ != nullptr && !audio_component_->IsPlaying("step"))
        {
            audio_component_->Play("step", true);
        }
    }
    else
    {
        if (audio_component_ != nullptr && audio_component_->IsPlaying("step"))
        {
            audio_component_->Stop("step");
        }
    }
}
