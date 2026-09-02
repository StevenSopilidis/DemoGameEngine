#pragma once

#include <glm/vec3.hpp>
#include <memory>

class btCollisionShape;

namespace engine
{

class Collider
{
  public:
    virtual ~Collider();

    btCollisionShape* GetShape();

  protected:
    std::unique_ptr<btCollisionShape> shape_;
};

class BoxCollider : public Collider
{
  public:
    BoxCollider(const glm::vec3& extents);
};

class SphereCollider : public Collider
{
  public:
    SphereCollider(float radius);
};

class CapsuleCollider : public Collider
{
  public:
    CapsuleCollider(float radius, float height);
};

} // namespace engine