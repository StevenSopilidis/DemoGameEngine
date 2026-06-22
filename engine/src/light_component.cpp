#include "light_component.h"

namespace engine
{
void LightComponent::Update(float deltaTime) {}

const glm::vec3& LightComponent::Color() const { return color_; }

void LightComponent::SetColor(const glm::vec3& color) { color_ = color; }

} // namespace engine