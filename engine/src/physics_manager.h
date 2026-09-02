#pragma once

#include <memory>
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace engine
{
class RigidBody;

class PhysicsManager
{
  public:
    PhysicsManager();
    ~PhysicsManager();

    void Init();
    void Update(float deltaTime);

    void AddRigidBody(RigidBody* body);
    void RemoveRigidBody(RigidBody* body);

    btDiscreteDynamicsWorld* GetWorld();

  private:
    std::unique_ptr<btBroadphaseInterface>               broadphase_;
    std::unique_ptr<btDefaultCollisionConfiguration>     collisionConfig_;
    std::unique_ptr<btCollisionDispatcher>               dispatcher_;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
    std::unique_ptr<btDiscreteDynamicsWorld>             world_;
};
} // namespace engine