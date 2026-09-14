#pragma once

#include "animation_component.h"
#include "engine.h"

class Player : public engine::GameObject
{
  public:
    void Init();
    void Update(float deltaTime) override;

  private:
    engine::AnimationComponent* animation_component_{};
};