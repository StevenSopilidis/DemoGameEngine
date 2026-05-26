#include "camera_component.h"

#include "game_object.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace engine
{
void CameraComponent::Update(float deltaTime) {}

glm::mat4 CameraComponent::GetViewMatrix() const
{
    auto mat = glm::mat4(1.0f);
    mat      = glm::mat4_cast(owner_->Rotation());
    mat[3]   = glm::vec4(owner_->Position(), 1.0f);

    if (owner_->Parent() != nullptr)
    {
        mat = owner_->GetWorldTransform() * mat;
    }

    return glm::inverse(mat);
}

glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
{
    return glm::perspective(glm::radians(fov_), aspect, near_plane_, far_plane_);
}

} // namespace engine