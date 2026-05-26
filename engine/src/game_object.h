#pragma once

#include "component.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>

namespace engine
{
class Scene;

class GameObject
{
  public:
    virtual ~GameObject() = default;

    GameObject(const GameObject&)            = delete;
    GameObject& operator=(const GameObject&) = delete;

    virtual void                     Update(float deltaTime);
    [[nodiscard]] const std::string& Name() const;
    void                             SetName(const std::string& name);
    [[nodiscard]] GameObject*        Parent() const;
    [[nodiscard]] bool               IsAlive() const;
    void                             MarkForDestroy();

    void AddComponent(Component* component);
    template <typename T, typename = typename std::enable_if<std::is_base_of_v<Component, T>>>
    T* GetComponent()
    {
        auto typeId = Component::StaticTypeId<T>();

        for (const auto& component : components_)
        {
            if (component->GetTypeId() == typeId)
            {
                return static_cast<T*>(component.get());
            }
        }

        return nullptr;
    }

    [[nodiscard]] const glm::vec3& Position() const;
    [[nodiscard]] const glm::quat& Rotation() const;
    [[nodiscard]] const glm::vec3& Scale() const;

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::quat rotation);
    void SetScale(glm::vec3 scale);

    [[nodiscard]] glm::mat4 GetLocalTransform() const;
    [[nodiscard]] glm::mat4 GetWorldTransform() const;

  protected:
    GameObject() = default;

  private:
    std::string                              name_;
    GameObject*                              parent_{nullptr};
    std::vector<std::unique_ptr<GameObject>> children_;
    std::vector<std::unique_ptr<Component>>  components_;
    bool                                     is_alive_{true};

    glm::vec3 position_ = glm::vec3(0.0f);
    glm::quat rotation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale_    = glm::vec3(1.0f);

    friend class Scene;
};
} // namespace engine