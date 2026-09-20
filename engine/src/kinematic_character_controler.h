#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace engine
{
class KinematicCharacterController
{
  public:
    KinematicCharacterController(float radius, float height, const glm::vec3& pos);
    ~KinematicCharacterController();

    [[nodiscard]] glm::vec3 GetPosition() const;
    [[nodiscard]] glm::quat GetRotation() const;

    void               Walk(const glm::vec3& direction);
    void               Jump(const glm::vec3& direction);
    [[nodiscard]] bool OnGround() const;

  private:
    float height_{1.2f};
    float radius_{0.4f};

    std::unique_ptr<btPairCachingGhostObject>       ghost_;
    std::unique_ptr<btKinematicCharacterController> controller_;
};
} // namespace engine