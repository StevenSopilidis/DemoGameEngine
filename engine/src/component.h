#pragma once

#include <cstddef>
namespace engine
{
class GameObject;

class Component
{
  public:
    virtual ~Component()                                 = default;
    virtual void                 Update(float deltaTime) = 0;
    [[nodiscard]] virtual size_t GetTypeId() const       = 0;

    [[nodiscard]] GameObject* Owner() const;

    template <typename T> static size_t StaticTypeId()
    {
        static size_t typeId = next_id_++;
        return typeId;
    }

  private:
    static size_t next_id_;

    GameObject* owner_{nullptr};

    friend class GameObject;
};

#define COMPONENT(ComponentClass)                                                                  \
  public:                                                                                          \
    static size_t        TypeId() { return Component::StaticTypeId<ComponentClass>(); }            \
    [[nodiscard]] size_t GetTypeId() const override { return TypeId(); }
} // namespace engine