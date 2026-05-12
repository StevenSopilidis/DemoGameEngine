#pragma once

#include "glm/ext/vector_float2.hpp"

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

    void SetMouseButtonPressed(int button, bool pressed);
    bool IsMouseButtonPressed(int button);

    void                           SetMousePositionOld(const glm::vec2& pos);
    [[nodiscard]] const glm::vec2& MousePositionOld() const;

    void                           SetMousePositionCurr(const glm::vec2& pos);
    [[nodiscard]] const glm::vec2& MousePositionCurr() const;

  private:
    friend class Engine;

    InputManager() = default;

    std::array<bool, 256> keyes_               = {false};
    std::array<bool, 16>  mouse_keys_          = {false};
    glm::vec2             mouse_position_old_  = glm::vec2(0.0f);
    glm::vec2             mouse_position_curr_ = glm::vec2(0.0f);
}; // namespace engine
} // namespace engine