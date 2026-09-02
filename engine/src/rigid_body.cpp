#include "rigid_body.h"

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "engine.h"

namespace engine
{
RigidBody::RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass,
                     float friction)
    : type_(type), collider_(collider), mass_(mass), friction_(friction)
{
    if (!collider)
    {
        return;
    }

    btVector3 inertia{0.0f, 0.0f, 0.0f};
    if (type == BodyType::Dynamic && mass > 0.f && collider_->GetShape())
    {
        collider_->GetShape()->calculateLocalInertia(static_cast<btScalar>(mass_), inertia);
    }

    btTransform transform;
    transform.setIdentity();
    auto* motionState = new btDefaultMotionState(transform);

    btRigidBody::btRigidBodyConstructionInfo info(
        (type_ == BodyType::Dynamic) ? static_cast<btScalar>(mass) : 0, motionState,
        collider_->GetShape(), inertia);

    body_ = std::make_unique<btRigidBody>(info);
    body_->setFriction(friction_);

    if (type_ == BodyType::Kinematic)
    {
        body_->setCollisionFlags(body_->getCollisionFlags() |
                                 btCollisionObject::CF_KINEMATIC_OBJECT);
        body_->setActivationState(DISABLE_DEACTIVATION);
    }
}

RigidBody::~RigidBody()
{
    if (added_to_world_)
    {
        Engine::GetInstance().GetPhysicsManager().RemoveRigidBody(this);
    }
}

btRigidBody* RigidBody::Body() const { return body_.get(); }

void RigidBody::SetAddedToWorld(bool added) { added_to_world_ = added; }

bool RigidBody::IsAddedToWorld() const { return added_to_world_; }

BodyType RigidBody::Type() const { return type_; }

void RigidBody::SetPosition(const glm::vec3& pos)
{
    if (!body_)
    {
        return;
    }

    auto& transform = body_->getWorldTransform();
    transform.setOrigin(btVector3{static_cast<btScalar>(pos.x), static_cast<btScalar>(pos.y),
                                  static_cast<btScalar>(pos.z)});

    if (body_->getMotionState() != nullptr)
    {
        body_->getMotionState()->setWorldTransform(transform);
    }
    body_->setWorldTransform(transform);
}

glm::vec3 RigidBody::Position() const
{
    const auto& pos = body_->getWorldTransform().getOrigin();
    return {pos.x(), pos.y(), pos.z()};
}

void RigidBody::SetRotation(const glm::quat& rot)
{
    if (!body_)
    {
        return;
    }

    auto& transform = body_->getWorldTransform();
    transform.setRotation(btQuaternion{static_cast<btScalar>(rot.x), static_cast<btScalar>(rot.y),
                                       static_cast<btScalar>(rot.z), static_cast<btScalar>(rot.w)});

    if (body_->getMotionState() != nullptr)
    {
        body_->getMotionState()->setWorldTransform(transform);
    }
    body_->setWorldTransform(transform);
}

glm::quat RigidBody::Rotation() const
{
    const auto& rot = body_->getWorldTransform().getRotation();
    return {rot.x(), rot.y(), rot.z(), rot.w()};
}

} // namespace engine