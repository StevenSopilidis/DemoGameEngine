#pragma once

#include "component.h"
#include "kinematic_character_controler.h"

#include <memory>

namespace engine
{

class PlayerControllerComponent : public Component
{
    COMPONENT(PlayerControllerComponent);

  public:
    void Init() override;
    void Update(float deltaTime) override;

  private:
    std::unique_ptr<KinematicCharacterController> controller_;

    float sensitivity_{12.0f};
    float move_speed_{30.0f};
    float y_rot_{};
    float x_rot_{};
};

} // namespace engine