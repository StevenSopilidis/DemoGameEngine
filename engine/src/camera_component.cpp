#include "camera_component.h"

#include "game_object.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace engine
{
void CameraComponent::Update(float deltaTime) {}

glm::mat4 CameraComponent::GetViewMatrix() const
{
    return glm::inverse(Owner()->GetWorldTransform());
}

glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
{
    return glm::perspective(glm::radians(fov_), aspect, near_plane_, far_plane_);
}

} // namespace engine