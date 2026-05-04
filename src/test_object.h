#pragma once

#include "engine_includes.h"

#include <memory>

class TestObject : public engine::GameObject
{
  public:
    TestObject();

    void Update(float deltaTime) override;

  private:
    engine::Material              material_;
    std::unique_ptr<engine::Mesh> mesh_;
};