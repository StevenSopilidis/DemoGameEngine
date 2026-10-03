#pragma once

#include "engine_includes.h"

class Bullet : public engine::GameObject
{
    GAMEOBJECT(Bullet)
  public:
    void Update(float deltaTime) override;

  private:
    float life_time_{2.0f};
};