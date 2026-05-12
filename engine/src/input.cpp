#include "input.h"

#include <iostream>

namespace engine
{
bool InputManager::IsKeyPressed(std::size_t key)
{
    if (key < 0 || key >= keyes_.size())
    {
        return false;
    }

    return keyes_[key];
}

void InputManager::SetKeyPressed(std::size_t key, bool pressed)
{
    if (key < 0 || key >= keyes_.size())
    {
        return;
    }

    keyes_[key] = pressed;
}

void InputManager::SetMouseButtonPressed(int button, bool pressed)
{
    if (button < 0 || button >= mouse_keys_.size())
    {
        return;
    }

    mouse_keys_[button] = pressed;
}

bool InputManager::IsMouseButtonPressed(int button)
{
    if (button < 0 || button >= mouse_keys_.size())
    {
        return false;
    }

    return mouse_keys_[button];
}

void InputManager::SetMousePositionOld(const glm::vec2& pos) { mouse_position_old_ = pos; }

[[nodiscard]] const glm::vec2& InputManager::MousePositionOld() const
{
    return mouse_position_old_;
}

void InputManager::SetMousePositionCurr(const glm::vec2& pos) { mouse_position_curr_ = pos; }

[[nodiscard]] const glm::vec2& InputManager::MousePositionCurr() const
{
    return mouse_position_curr_;
}

} // namespace engine