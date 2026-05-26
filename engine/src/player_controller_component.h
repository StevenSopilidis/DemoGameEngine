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
    float sensitivity_{0.3f};
    float move_speed_{1.4f};
};
} // namespace engine