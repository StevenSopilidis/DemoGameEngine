#pragma once

#include "animation_component.h"
#include "audio_component.h"
#include "engine.h"
#include "player_controller_component.h"

class Player : public engine::GameObject
{
    GAMEOBJECT(Player)
  public:
    void Init() override;
    void Update(float deltaTime) override;

  private:
    engine::AnimationComponent*        animation_component_{};
    engine::AudioComponent*            audio_component_{};
    engine::PlayerControllerComponent* player_controller_component_{};
};