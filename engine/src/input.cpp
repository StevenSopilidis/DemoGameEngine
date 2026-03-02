#include "input.h"

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
} // namespace engine