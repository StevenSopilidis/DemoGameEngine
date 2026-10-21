#include "jump_platform.h"

#include "physics_component.h"

void JumpPlatform::Init()
{
    auto* physics = GetComponent<engine::PhysicsComponent>();
    if (physics != nullptr)
    {
        auto rigidBody = physics->GetRigitBody();
        if (rigidBody != nullptr)
        {
            rigidBody->AddContactListener(this);
        }
    }
}

void JumpPlatform::OnContact(engine::CollisionObject* obj, const glm::vec3& pos,
                             const glm::vec3& norm)
{
    if (obj->GetCollisionObjectType() == engine::CollisionObjectType::KinematicCharacterController)
    {
        auto* controller = static_cast<engine::KinematicCharacterController*>(obj);
        if (controller != nullptr)
        {
            controller->Jump(glm::vec3(0.0f, 20.0f, 0.0f));
        }
    }
}
