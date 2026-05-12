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
        rotation.y -= deltaX * sensitivity_ * deltaTime;

        // rotation around X axis
        rotation.y -= deltaY * sensitivity_ * deltaTime;

        owner_->SetRotation(rotation);
    }

    glm::mat4 rotMat(1.0f);

    rotMat = glm::rotate(rotMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // x-axis
    rotMat = glm::rotate(rotMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // y-axis
    rotMat = glm::rotate(rotMat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // z-axis

    glm::vec3 front = glm::normalize(glm::vec3(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
    glm::vec3 right = glm::normalize(glm::vec3(rotMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));

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