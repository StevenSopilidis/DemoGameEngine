#include "game_object.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{
void GameObject::Update(float deltaTime)
{
    auto result = std::ranges::remove_if(children_,
                                         [](const std::unique_ptr<GameObject>& child)
                                         {
                                             return !child->IsAlive(); // usually remove dead ones
                                         });

    children_.erase(result.begin(), result.end());

    for (auto& child : children_)
    {
        child->Update(deltaTime);
    }
}

const std::string& GameObject::Name() const { return name_; };

void GameObject::SetName(const std::string& name) { name_ = name; }

GameObject* GameObject::Parent() const { return parent_; }

bool GameObject::IsAlive() const { return is_alive_; }

void GameObject::MarkForDestroy() { is_alive_ = false; }

[[nodiscard]] const glm::vec3& GameObject::Position() const { return position_; }
[[nodiscard]] const glm::vec3& GameObject::Rotation() const { return rotation_; }
[[nodiscard]] const glm::vec3& GameObject::Scale() const { return scale_; }

void GameObject::SetPosition(glm::vec3 position) { position_ = position; }
void GameObject::SetRotation(glm::vec3 rotation) { rotation_ = rotation; }
void GameObject::SetScale(glm::vec3 scale) { scale_ = scale; };

[[nodiscard]] glm::mat4 GameObject::GetLocalTransform() const
{
    auto mat = glm::mat4(1.0f);
    mat      = glm::translate(mat, position_);

    mat = glm::rotate(mat, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f)); // x-axis
    mat = glm::rotate(mat, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f)); // y-axis
    mat = glm::rotate(mat, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f)); // z-axis

    mat = glm::scale(mat, scale_);

    return mat;
}

[[nodiscard]] glm::mat4 GameObject::GetWorldTransform() const
{
    if (parent_ != nullptr)
    {
        return parent_->GetWorldTransform() * GetLocalTransform();
    }

    return GetLocalTransform();
}

} // namespace engine