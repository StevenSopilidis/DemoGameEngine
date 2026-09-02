#include "physics_component.h"

#include "engine.h"
#include "game_object.h"

namespace engine
{

PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& body) : rigidBody_(body) {}

void PhysicsComponent::Init()
{
    if (!rigidBody_)
    {
        return;
    }

    const auto pos = owner_->GetWorldPosition();
    const auto rot = owner_->Rotation();

    rigidBody_->SetPosition(pos);
    rigidBody_->SetRotation(rot);

    Engine::GetInstance().GetPhysicsManager().AddRigidBody(rigidBody_.get());
}

void PhysicsComponent::Update(float deltaTime)
{
    if (!rigidBody_)
    {
        return;
    }

    if (rigidBody_->Type() == BodyType::Dynamic)
    {
        owner_->SetPosition(rigidBody_->Position());
        owner_->SetRotation(rigidBody_->Rotation());
    }
}

} // namespace engine