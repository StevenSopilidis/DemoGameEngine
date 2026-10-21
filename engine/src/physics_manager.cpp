#include "physics_manager.h"

#include "collision_object.h"
#include "rigid_body.h"

#include <btBulletDynamicsCommon.h>

namespace engine
{

PhysicsManager::~PhysicsManager() {}

PhysicsManager::PhysicsManager() = default;

void PhysicsManager::Init()
{
    broadphase_      = std::make_unique<btDbvtBroadphase>();
    collisionConfig_ = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher_      = std::make_unique<btCollisionDispatcher>(collisionConfig_.get());
    solver_          = std::make_unique<btSequentialImpulseConstraintSolver>();
    world_ = std::make_unique<btDiscreteDynamicsWorld>(dispatcher_.get(), broadphase_.get(),
                                                       solver_.get(), collisionConfig_.get());

    world_->setGravity(btVector3(0, -9.81f, 0));
}

void PhysicsManager::Update(float deltaTime)
{
    const btScalar fixedTimeStep = 1.0f / 60.0f;
    const auto     maxSubsteps   = 4;
    world_->stepSimulation(deltaTime, maxSubsteps, fixedTimeStep);

    // process collisions
    auto* dispatcher = world_->getDispatcher();
    for (int i{}; i < dispatcher->getNumManifolds(); i++)
    {
        auto* manifold = dispatcher->getManifoldByIndexInternal(i);
        if (manifold == nullptr)
        {
            continue;
        }

        auto* bodyA = reinterpret_cast<CollisionObject*>(manifold->getBody0()->getUserPointer());
        auto* bodyB = reinterpret_cast<CollisionObject*>(manifold->getBody1()->getUserPointer());

        if (bodyA == nullptr || bodyB == nullptr)
        {
            continue;
        }

        const auto numContacts = manifold->getNumContacts();
        for (int j{}; j < numContacts; j++)
        {
            const auto& point = manifold->getContactPoint(j);

            const glm::vec3 pos(point.m_positionWorldOnB.x(), point.m_positionWorldOnB.y(),
                                point.m_positionWorldOnB.z());

            const glm::vec3 norm(point.m_normalWorldOnB.x(), point.m_normalWorldOnB.y(),
                                 point.m_normalWorldOnB.z());

            bodyA->DispatchContactEvent(bodyB, pos, norm);
            bodyB->DispatchContactEvent(bodyA, pos, norm);
        }
    }
}

btDiscreteDynamicsWorld* PhysicsManager::GetWorld() { return world_.get(); }

void PhysicsManager::AddRigidBody(RigidBody* body)
{
    if (!body || !world_)
    {
        return;
    }

    if (auto rigidBody = body->Body())
    {
        world_->addRigidBody(rigidBody, btBroadphaseProxy::StaticFilter,
                             btBroadphaseProxy::AllFilter);
        body->SetAddedToWorld(true);
    }
}

void PhysicsManager::RemoveRigidBody(RigidBody* body)
{
    if (!body || !world_)
    {
        return;
    }

    if (auto rigidBody = body->Body())
    {
        world_->removeRigidBody(rigidBody);
        body->SetAddedToWorld(false);
    }
}

} // namespace engine