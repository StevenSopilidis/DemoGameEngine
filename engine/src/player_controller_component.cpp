#include "player_controller_component.h"

#include "engine.h"

#include "GLFW/glfw3.h"
#include "component.h"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace engine
{
void PlayerControllerComponent::Update(float deltaTime)
{
    auto& inputManager = Engine::GetInstance().GetInputManager();
    auto  rotation     = owner_->Rotation();

    if (inputManager.IsMousePositionChanged())
    {
        const auto& oldPos  = inputManager.MousePositionOld();
        const auto& currPos = inputManager.MousePositionCurr();

        float deltaX = currPos.x - oldPos.x;
        float deltaY = currPos.y - oldPos.y;

        // rotation around Y axis
        auto yDeltaAngle = -deltaX * sensitivity_ * deltaTime;
        y_rot_ += yDeltaAngle;
        auto yRot = glm::angleAxis(glm::radians(y_rot_), glm::vec3(0.0, 1.0f, 0.0f));

        // rotation around X axis
        auto xDeltaAngle = -deltaY * sensitivity_ * deltaTime;
        x_rot_ += xDeltaAngle;
        x_rot_    = std::clamp(x_rot_, -89.0f, 89.0f);
        auto xRot = glm::angleAxis(glm::radians(x_rot_), glm::vec3(1.0f, 0.0f, 0.0f));

        rotation = glm::normalize(yRot * xRot);
        owner_->SetRotation(rotation);
    }

    if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        const auto& oldPos  = inputManager.MousePositionOld();
        const auto& currPos = inputManager.MousePositionCurr();

        float deltaY = currPos.y - oldPos.y;

        auto up       = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
        auto position = owner_->Position();
        position += deltaY * up * move_speed_ * deltaTime;
        owner_->SetPosition(position);
    }

    auto front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    auto right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);

    auto position = owner_->Position();

    glm::vec3 move(0.0f);
    // LEFT/RIGHT movement
    if (inputManager.IsKeyPressed(GLFW_KEY_A))
    {
        move -= right;
    }
    else if (inputManager.IsKeyPressed(GLFW_KEY_D))
    {
        move += right;
    }
    // vertical movement
    else if (inputManager.IsKeyPressed(GLFW_KEY_W))
    {
        move += front;
    }
    else if (inputManager.IsKeyPressed(GLFW_KEY_S))
    {
        move -= front;
    }

    if (glm::dot(move, move) > 0)
    {
        move = glm::normalize(move);
    }
    controller_->Walk(move * move_speed_ * deltaTime);

    if (inputManager.IsKeyPressed(GLFW_KEY_SPACE))
    {
        controller_->Jump(glm::vec3(0.0f, 5.0f, 0.0f));
    }

    owner_->SetPosition(controller_->GetPosition());
}

void PlayerControllerComponent::Init()
{
    controller_ =
        std::make_unique<KinematicCharacterController>(0.4f, 1.2f, owner_->GetWorldPosition());
}

} // namespace engine