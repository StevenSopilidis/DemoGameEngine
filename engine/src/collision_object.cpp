#include "collision_object.h"

#include <algorithm>

namespace engine
{

CollisionObjectType CollisionObject::GetCollisionObjectType() const
{
    return collision_object_type_;
}

void CollisionObject::AddContactListener(IContactListener* listener)
{
    listeners_.push_back(listener);
}

void CollisionObject::RemoveContactListener(IContactListener* listener)
{
    auto it = std::ranges::find(listeners_, listener);
    if (it != listeners_.end())
    {
        listeners_.erase(it);
    }
}

void CollisionObject::DispatchContactEvent(CollisionObject* obj, const glm::vec3& pos,
                                           const glm::vec3& norm)
{
    for (auto* listener : listeners_)
    {
        if (listener != nullptr)
        {
            listener->OnContact(obj, pos, norm);
        }
    }
}

} // namespace engine