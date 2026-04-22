#pragma once

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

  protected:
    GameObject() = default;

  private:
    std::string                              name_;
    GameObject*                              parent_{nullptr};
    std::vector<std::unique_ptr<GameObject>> children_;
    bool                                     is_alive_{true};

    friend class Scene;
};
} // namespace engine