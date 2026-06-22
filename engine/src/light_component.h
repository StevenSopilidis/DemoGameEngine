#pragma once

#include "component.h"

#include <glm/vec3.hpp>

namespace engine
{
class LightComponent : public Component
{
    COMPONENT(LightComponent)
  public:
    void Update(float deltaTime) override;

    [[nodiscard]] const glm::vec3& Color() const;
    void                           SetColor(const glm::vec3& color);

  private:
    glm::vec3 color_ = glm::vec3(1.0f);
};
} // namespace engine