#pragma once

namespace engine
{
class GameObject;

class Component
{
  public:
    virtual ~Component()                 = default;
    virtual void Update(float deltaTime) = 0;

    GameObject* Owner();

  private:
    GameObject* owner_{nullptr};

    friend class GameObject;
};
} // namespace engine