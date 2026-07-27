#pragma once

#include "component.h"

namespace engine
{
class PlayerControllerComponent : public Component
{
    COMPONENT(PlayerControllerComponent);

  public:
    void Update(float deltaTime) override;

  private:
    float sensitivity_{2.0f};
    float move_speed_{3.0f};
};
} // namespace engine