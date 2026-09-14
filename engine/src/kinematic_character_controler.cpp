#include "kinematic_character_controler.h"

#include "engine.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <btBulletDynamicsCommon.h>

namespace engine
{

KinematicCharacterController::KinematicCharacterController(float radius, float height)
    : radius_(radius), height_(height)
{
    auto world   = Engine::GetInstance().GetPhysicsManager().GetWorld();
    auto capsule = new btCapsuleShape(radius_, height_);

    ghost_ = std::make_unique<btPairCachingGhostObject>();
    btTransform start;
    start.setIdentity();
    start.setOrigin(btVector3(0.0f, 2.0f, 0.0f));
    ghost_->setWorldTransform(start);
    ghost_->setCollisionShape(capsule);
    ghost_->setCollisionFlags(ghost_->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);

    world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(
        new btGhostPairCallback());

    const btScalar stepHeight{0.35f};
    controller_ =
        std::make_unique<btKinematicCharacterController>(ghost_.get(), capsule, stepHeight);

    controller_->setMaxSlope(btRadians(50.f));
    controller_->setGravity(world->getGravity());

    world->addCollisionObject(ghost_.get(), btBroadphaseProxy::CharacterFilter,
                              btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
    world->addAction(controller_.get());
}

KinematicCharacterController::~KinematicCharacterController()
{
    auto world = Engine::GetInstance().GetPhysicsManager().GetWorld();

    if (controller_)
    {
        world->removeAction(controller_.get());
    }

    if (ghost_)
    {
        world->removeCollisionObject(ghost_.get());
    }
}

glm::vec3 KinematicCharacterController::GetPosition() const
{
    const auto&     pos = ghost_->getWorldTransform().getOrigin();
    const glm::vec3 offset(0.0f, height_ + 0.5f + radius_, 0.0f);
    return glm::vec3(pos.x(), pos.y(), pos.z()) + offset;
}

glm::quat KinematicCharacterController::GetRotation() const
{
    const auto& rot = ghost_->getWorldTransform().getRotation();
    return {rot.w(), rot.x(), rot.y(), rot.z()};
}

void KinematicCharacterController::Walk(const glm::vec3& direction)
{
    controller_->setWalkDirection({direction.x, direction.y, direction.z});
}

void KinematicCharacterController::Jump(const glm::vec3& direction)
{
    if (controller_->onGround())
    {
        controller_->jump({direction.x, direction.y, direction.z});
    }
}

bool KinematicCharacterController::OnGround() const { return controller_->onGround(); }

} // namespace engine