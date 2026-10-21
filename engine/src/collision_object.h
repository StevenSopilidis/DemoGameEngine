#pragma once

#include "glm/ext/vector_float3.hpp"

namespace engine
{
class IContactListener;

enum class CollisionObjectType
{
    RigidBody,
    KinematicCharacterController
};

class CollisionObject
{
  public:
    [[nodiscard]] CollisionObjectType GetCollisionObjectType() const;

    void AddContactListener(IContactListener* listener);
    void RemoveContactListener(IContactListener* listener);

  protected:
    void DispatchContactEvent(CollisionObject* obj, const glm::vec3& pos, const glm::vec3& norm);

    CollisionObjectType            collision_object_type_;
    std::vector<IContactListener*> listeners_;

    friend class PhysicsManager;
};

class IContactListener
{
  public:
    virtual void OnContact(CollisionObject* obj, const glm::vec3& pos, const glm::vec3& norm) = 0;
};

} // namespace engine