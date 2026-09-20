#pragma once

#include "component.h"

#include <filesystem>
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

    virtual void Init();
    virtual void LoadProperties(const nlohmann::json& json);

    virtual void                     Update(float deltaTime);
    [[nodiscard]] const std::string& Name() const;
    void                             SetName(const std::string& name);
    [[nodiscard]] GameObject*        Parent() const;
    bool                             SetParent(GameObject* parent);
    Scene*                           GetScene();
    [[nodiscard]] bool               IsAlive() const;
    void                             MarkForDestroy();
    void                             SetActive(bool active);
    [[nodiscard]] bool               IsActive() const;

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

    GameObject* FindChildByName(const std::string& name);

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::quat rotation);
    void SetScale(glm::vec3 scale);

    void SetWorldPosition(const glm::vec3& pos);

    [[nodiscard]] glm::quat GetWorldRotation() const;
    void                    SetWorldRotation(const glm::quat& rot);

    [[nodiscard]] glm::mat4 GetLocalTransform() const;
    [[nodiscard]] glm::mat4 GetWorldTransform() const;

    [[nodiscard]] glm::vec3 GetWorldPosition() const;

    static GameObject* LoadGLTF(const std::filesystem::path& path, Scene* gameScene);

  protected:
    GameObject() = default;

    std::string                              name_;
    Scene*                                   scene_{};
    GameObject*                              parent_{};
    std::vector<std::unique_ptr<GameObject>> children_;
    std::vector<std::unique_ptr<Component>>  components_;
    bool                                     is_alive_{true};
    bool                                     is_active_{true};

    glm::vec3 position_ = glm::vec3(0.0f);
    glm::quat rotation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale_    = glm::vec3(1.0f);

    friend class Scene;
};

class ObjectCreatorBase
{
  public:
    virtual ~ObjectCreatorBase()           = default;
    virtual GameObject* CreateGameObject() = 0;
};

template <typename T> class ObjectCreator : public ObjectCreatorBase
{
  public:
    GameObject* CreateGameObject() override { return new T(); }
};

class GameObjectFactory
{
  public:
    static GameObjectFactory& GetInstance();

    template <typename T> void RegisterObject(const std::string& name)
    {
        creators_.emplace(name, std::make_unique<ObjectCreator<T>>());
    }

    GameObject* CreateGameObject(const std::string& typeName);

  private:
    std::unordered_map<std::string, std::unique_ptr<ObjectCreatorBase>> creators_;
};

#define GAMEOBJECT(ObjectClass)                                                                    \
  public:                                                                                          \
    static void Register()                                                                         \
    {                                                                                              \
        engine::GameObjectFactory::GetInstance().RegisterObject<ObjectClass>(                      \
            std::string(#ObjectClass));                                                            \
    }

} // namespace engine