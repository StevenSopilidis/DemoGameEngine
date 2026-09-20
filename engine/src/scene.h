#pragma once

#include "common.h"
#include "game_object.h"
#include "nlohmann/json.hpp"

#include <memory>
#include <vector>

namespace engine
{

class Scene
{
  public:
    static void RegisterTypes();

    void Update(float deltaTime);
    void Clear();

    GameObject* CreateObject(const std::string& name, GameObject* parent = nullptr);
    GameObject* CreateObject(const std::string& type, const std::string& name,
                             GameObject* parent = nullptr);

    template <typename T>
    T* CreateObject(const std::string& name, GameObject* parent = nullptr)
        requires(std::is_base_of_v<GameObject, T>)
    {
        auto obj = new T();
        obj->SetName(name);
        obj->scene_ = this;
        SetParent(obj, parent);

        return obj;
    }

    bool SetParent(GameObject* obj, GameObject* parent);

    void                      SetMainCamera(GameObject* camera);
    [[nodiscard]] GameObject* MainCamera() const;

    std::vector<LightData> CollectLights();

    static std::shared_ptr<Scene> Load(const std::filesystem::path& path);

  private:
    void CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out);

    void LoadObject(const nlohmann::json& jsonObject, GameObject* parent);

    std::vector<std::unique_ptr<GameObject>> objects_;
    GameObject*                              main_camera_{nullptr};
};
} // namespace engine