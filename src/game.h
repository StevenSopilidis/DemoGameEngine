#pragma once

#include "engine_includes.h"

class Game : public engine::Application
{
  public:
    bool Init() override;
    void Update(float deltaTime) override;
    void Destroy() override;
};