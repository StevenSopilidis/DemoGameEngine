#pragma once

#include "engine_includes.h"

class Game : public engine::Application
{
  public:
    void RegisterTypes() override;
    bool Init() override;
    void Update(float deltaTime) override;
    void Destroy() override;

  private:
    engine::Scene* scene_{nullptr};
};