#include "collider.h"

#include "BulletCollision/CollisionShapes/btCapsuleShape.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

namespace engine
{
Collider::~Collider() = default;

btCollisionShape* Collider::GetShape() { return shape_.get(); }

BoxCollider::BoxCollider(const glm::vec3& extents)
{
    glm::vec3 halfExtends = extents * 0.5f;
    shape_ = std::make_unique<btBoxShape>(btVector3(halfExtends.x, halfExtends.y, halfExtends.z));
}

SphereCollider::SphereCollider(float radius) { shape_ = std::make_unique<btSphereShape>(radius); }

CapsuleCollider::CapsuleCollider(float radius, float height)
{
    shape_ = std::make_unique<btCapsuleShape>(radius, height);
}

} // namespace engine