#pragma once

#include "component.h"
#include "glm/ext/matrix_float4x4.hpp"
namespace engine
{
class CameraComponent : public Component
{
    COMPONENT(CameraComponent)

  public:
    void Update(float deltaTime) override;

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    [[nodiscard]] glm::mat4 GetProjectionMatrix(float aspect) const;

  private:
    float fov_{60.f};
    float near_plane_{0.1f};
    float far_plane_{1000.f};
};
} // namespace engine