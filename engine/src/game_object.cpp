#include "game_object.h"

#include <algorithm>

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

} // namespace engine