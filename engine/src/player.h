#pragma once

#include "animation_component.h"
#include "engine.h"

class Player : public engine::GameObject
{
    GAMEOBJECT(Player)
  public:
    void Init() override;
    void Update(float deltaTime) override;

  private:
    engine::AnimationComponent* animation_component_{};
};