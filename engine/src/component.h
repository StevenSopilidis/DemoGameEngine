#pragma once

#include "nlohmann/json.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine
{
class GameObject;

class Component
{
  public:
    virtual ~Component() = default;
    virtual void                 LoadProperties(const nlohmann::json& json);
    virtual void                 Update(float deltaTime) = 0;
    virtual void                 Init();
    [[nodiscard]] virtual size_t GetTypeId() const = 0;

    [[nodiscard]] GameObject* Owner() const;

    template <typename T> static size_t StaticTypeId()
    {
        static size_t typeId = next_id_++;
        return typeId;
    }

  protected:
    GameObject* owner_{nullptr};
    friend class GameObject;

  private:
    static size_t next_id_;
};

class ComponentCreatorBase
{
  public:
    virtual ~ComponentCreatorBase()      = default;
    virtual Component* CreateComponent() = 0;
};

template <typename T> class ComponentCreator : public ComponentCreatorBase
{
  public:
    Component* CreateComponent() override { return new T(); }
};

class ComponentFactory
{
  public:
    static ComponentFactory& GetInstance();

    template <typename T> void RegisterComponent(const std::string& name)
    {
        creators_.emplace(name, std::make_unique<ComponentCreator<T>>());
    }

    Component* CreateComponent(const std::string& name)
    {
        auto it = creators_.find(name);
        if (it != creators_.end())
        {
            return it->second->CreateComponent();
        }

        return nullptr;
    }

  private:
    std::unordered_map<std::string, std::unique_ptr<ComponentCreatorBase>> creators_;
};

#define COMPONENT(ComponentClass)                                                                  \
  public:                                                                                          \
    static size_t        TypeId() { return engine::Component::StaticTypeId<ComponentClass>(); }    \
    [[nodiscard]] size_t GetTypeId() const override { return TypeId(); }                           \
    static void          Register()                                                                \
    {                                                                                              \
        engine::ComponentFactory::GetInstance().RegisterComponent<ComponentClass>(                 \
            std::string(#ComponentClass));                                                         \
    }
} // namespace engine