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

    if (inputManager.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        const auto& oldPos  = inputManager.MousePositionOld();
        const auto& currPos = inputManager.MousePositionCurr();

        float deltaX = currPos.x - oldPos.x;
        float deltaY = currPos.y - oldPos.y;

        // rotation around Y axis
        auto yAngle = -deltaX * sensitivity_ * deltaTime;
        auto yRot   = glm::angleAxis(yAngle, glm::vec3(0.0, 1.0f, 0.0f));

        // rotation around X axis
        auto xAngle = -deltaY * sensitivity_ * deltaTime;
        auto right  = rotation * glm::vec3(1.0, 0.0f, 0.0f);
        auto xRot   = glm::angleAxis(xAngle, right);

        auto deltaRot = yRot * xRot;
        rotation      = glm::normalize(deltaRot * rotation);

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

    // LEFT/RIGHT movement
    if (inputManager.IsKeyPressed(GLFW_KEY_A))
    {
        position -= right * move_speed_ * deltaTime;
    }
    else if (inputManager.IsKeyPressed(GLFW_KEY_D))
    {
        position += right * move_speed_ * deltaTime;
    }

    // vertical movement
    else if (inputManager.IsKeyPressed(GLFW_KEY_W))
    {
        position += front * move_speed_ * deltaTime;
    }
    else if (inputManager.IsKeyPressed(GLFW_KEY_S))
    {
        position -= front * move_speed_ * deltaTime;
    }

    owner_->SetPosition(position);
}
} // namespace engine