#pragma once

#include "game_object.h"

#include <memory>
#include <vector>

namespace engine
{

class Scene
{
  public:
    void Update(float deltaTime);
    void Clear();

    GameObject* CreateObject(const std::string& name, GameObject* parent = nullptr);

    template <typename T>
    T* CreateObject(const std::string& name, GameObject* parent = nullptr)
        requires(std::is_base_of_v<GameObject, T>)
    {
        auto obj = new T();
        obj->SetName(name);
        SetParent(obj, parent);

        return obj;
    }

    bool SetParent(GameObject* obj, GameObject* parent);

  private:
    std::vector<std::unique_ptr<GameObject>> objects_;
};
} // namespace engine