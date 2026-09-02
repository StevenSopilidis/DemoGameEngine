#pragma once

#include "collider.h"
#include "glm/gtc/quaternion.hpp"

#include <cstdint>
#include <memory>

class btRigidBody;

namespace engine
{
enum class BodyType : uint8_t
{
    Static,
    Dynamic,
    Kinematic
};

class RigidBody
{
  public:
    RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass, float friction);
    ~RigidBody();

    [[nodiscard]] btRigidBody* Body() const;

    void               SetAddedToWorld(bool added);
    [[nodiscard]] bool IsAddedToWorld() const;

    [[nodiscard]] BodyType Type() const;

    void                    SetPosition(const glm::vec3& pos);
    [[nodiscard]] glm::vec3 Position() const;

    void                    SetRotation(const glm::quat& pos);
    [[nodiscard]] glm::quat Rotation() const;

  private:
    std::unique_ptr<btRigidBody> body_;
    BodyType                     type_{BodyType::Static};
    std::shared_ptr<Collider>    collider_;
    float                        mass_{};
    float                        friction_{0.5f};
    bool                         added_to_world_{};
};

} // namespace engine