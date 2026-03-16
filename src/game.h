#pragma once

#include "engine_includes.h"

#include <memory.h>

class Game : public engine::Application
{
  public:
    bool Init() override;
    void Update(float deltaTime) override;
    void Destroy() override;

  private:
    engine::Material              material_;
    std::unique_ptr<engine::Mesh> mesh_;
    float                         offset_x_{0.0f};
    float                         offset_y_{0.0f};
};