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
    float sensitivity_{0.1f};
    float move_speed_{1.0f};
};
} // namespace engine