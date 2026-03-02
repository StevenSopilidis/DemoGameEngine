#pragma once

#include <array>
#include <cstddef>

namespace engine
{
class Engine;

class InputManager
{
  public:
    InputManager(const InputManager&)            = delete;
    InputManager(InputManager&&)                 = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager& operator=(InputManager&&)      = delete;

    void SetKeyPressed(std::size_t key, bool pressed);
    bool IsKeyPressed(std::size_t key);

  private:
    friend class Engine;

    InputManager() = default;

    std::array<bool, 256> keyes_ = {false};
}; // namespace engine
} // namespace engine