#include "scene.h"

#include "engine.h"
#include "light_component.h"

#include <algorithm>
#include <animation_component.h>
#include <camera_component.h>
#include <iostream>
#include <light_component.h>
#include <mesh_component.h>
#include <physics_component.h>
#include <player_controller_component.h>

namespace engine
{

void Scene::RegisterTypes()
{
    AnimationComponent::Register();
    CameraComponent::Register();
    LightComponent::Register();
    MeshComponent::Register();
    PhysicsComponent::Register();
    PlayerControllerComponent::Register();
}

void Scene::Update(float deltaTime)
{
    auto result = std::ranges::remove_if(objects_, [](const std::unique_ptr<GameObject>& obj)
                                         { return !obj->IsAlive(); });

    objects_.erase(result.begin(), result.end());

    for (auto& obj : objects_)
    {
        obj->Update(deltaTime);
    }
}

void Scene::Clear() { objects_.clear(); }

GameObject* Scene::CreateObject(const std::string& name, GameObject* parent)
{
    auto* obj = new GameObject();
    obj->SetName(name);
    obj->scene_ = this;
    SetParent(obj, parent);

    return obj;
}

GameObject* Scene::CreateObject(const std::string& type, const std::string& name,
                                GameObject* parent)
{
    auto* obj = GameObjectFactory::GetInstance().CreateGameObject(type);
    if (obj)
    {
        obj->SetName(name);
        obj->scene_ = this;
        SetParent(obj, parent);
    }

    return obj;
}

bool Scene::SetParent(GameObject* obj, GameObject* parent)
{
    auto res           = false;
    auto currentParent = obj->Parent();

    if (parent == nullptr)
    {
        // want to move to root of scene
        if (currentParent != nullptr)
        {
            auto it = std::ranges::find_if(currentParent->children_,
                                           [obj](const std::unique_ptr<GameObject>& el)
                                           { return el.get() == obj; });

            // check if children is list of currentParent and remove it
            if (it != currentParent->children_.end())
            {
                objects_.push_back(std::move(*it));
                obj->parent_ = nullptr;
                currentParent->children_.erase(it);
                res = true;
            }
        }
        else
        {
            // object has no parent, reasons:
            // 1) Object is in scene root
            // 2) Object just created
            auto it = std::ranges::find_if(objects_, [obj](const std::unique_ptr<GameObject>& el)
                                           { return el.get() == obj; });

            if (it == objects_.end())
            {
                // just created
                std::unique_ptr<GameObject> objHolder(obj);
                objects_.push_back(std::move(objHolder));
                res = true;
            }
        }
    }
    else
    {
        // move as child of another object
        if (currentParent != nullptr)
        {
            // object has already a parent
            auto it = std::ranges::find_if(currentParent->children_,
                                           [obj](const std::unique_ptr<GameObject>& el)
                                           { return el.get() == obj; });

            if (it != currentParent->children_.end())
            {
                auto  found          = false;
                auto* currentElement = parent;
                while (currentElement)
                {
                    if (currentElement == obj)
                    {
                        found = true;
                        break;
                    }
                    currentElement = currentElement->Parent();
                }

                if (!found)
                {
                    parent->children_.push_back(std::move(*it));
                    obj->parent_ = parent;
                    currentParent->children_.erase(it);
                    res = true;
                }
            }
        }
        else
        {
            // object has no parent, reasons:
            // 1) Object is in scene root
            // 2) Object just created
            auto it = std::ranges::find_if(objects_, [obj](const std::unique_ptr<GameObject>& el)
                                           { return el.get() == obj; });

            if (it == objects_.end())
            {
                // object just created
                std::unique_ptr<GameObject> objHolder(obj);
                parent->children_.push_back(std::move(objHolder));
                obj->parent_ = parent;
                res          = true;
            }
            else
            {
                // object in scene root
                auto  found          = false;
                auto* currentElement = parent;
                while (currentElement)
                {
                    if (currentElement == obj)
                    {
                        found = true;
                        break;
                    }
                    currentElement = currentElement->Parent();
                }

                if (!found)
                {
                    parent->children_.push_back(std::move((*it)));
                    obj->parent_ = parent;
                    objects_.erase(it);
                    res = true;
                }
            }
        }
    }

    return res;
}

void Scene::SetMainCamera(GameObject* camera) { main_camera_ = camera; }

GameObject* Scene::MainCamera() const { return main_camera_; }

std::vector<LightData> Scene::CollectLights()
{
    std::vector<LightData> lights;

    for (auto& obj : objects_)
    {
        CollectLightsRecursive(obj.get(), lights);
    }

    return lights;
}

void Scene::CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out)
{
    if (auto light = obj->GetComponent<LightComponent>())
    {
        LightData data;
        data.color = light->Color();
        data.pos   = obj->GetWorldPosition();
        out.push_back(data);
    }

    for (auto& child : obj->children_)
    {
        CollectLightsRecursive(child.get(), out);
    }
}

std::shared_ptr<Scene> Scene::Load(const std::filesystem::path& path)
{
    const auto content = Engine::GetInstance().GetFs().LoadAssetFile(path);
    if (content.empty())
    {
        return nullptr;
    }

    auto json = nlohmann::json::parse(content);
    if (json.empty())
    {
        return nullptr;
    }

    auto result = std::make_shared<Scene>();

    const auto sceneName = json.value("name", "noname");
    if (json.contains("objects") && json["objects"].is_array())
    {
        const auto& objects = json["objects"];

        for (const auto& obj : objects)
        {
            result->LoadObject(obj, nullptr);
        }
    }

    if (json.contains("camera"))
    {
        auto cameraObjName = json.value("camera", "");
        for (const auto& child : result->objects_)
        {
            if (auto object = child->FindChildByName(cameraObjName))
            {
                result->SetMainCamera(object);
                break;
            }
        }
    }

    return result;
}

void Scene::LoadObject(const nlohmann::json& jsonObject, GameObject* parent)
{
    const auto  name = jsonObject.value("name", "no-name-obj");
    GameObject* gameObject{};

    if (jsonObject.contains("type"))
    {
        const auto type = jsonObject.value("type", "");
        if (type == "gltf")
        {
            auto path  = jsonObject.value("path", "");
            gameObject = GameObject::LoadGLTF(path, this);
            if (gameObject)
            {
                gameObject->SetParent(parent);
                gameObject->SetName(name);
            }
        }
        else
        {
            gameObject = CreateObject(type, name, parent);
        }
    }
    else
    {
        gameObject = CreateObject(name, parent);
    }

    if (gameObject == nullptr)
    {
        return;
    }

    if (jsonObject.contains("position"))
    {
        const auto& posObj = jsonObject["position"];
        glm::vec3   pos;
        pos.x = posObj.value("x", 0.0f);
        pos.y = posObj.value("y", 0.0f);
        pos.z = posObj.value("z", 0.0f);
        gameObject->SetPosition(pos);
    }

    if (jsonObject.contains("rotation"))
    {
        const auto& rotObj = jsonObject["rotation"];
        glm::quat   rot;
        rot.x = rotObj.value("x", 0.0f);
        rot.y = rotObj.value("y", 0.0f);
        rot.z = rotObj.value("z", 0.0f);
        rot.w = rotObj.value("w", 1.0f);
        gameObject->SetRotation(rot);
    }

    if (jsonObject.contains("scale"))
    {
        const auto& scaleObj = jsonObject["scale"];
        glm::vec3   scale;
        scale.x = scaleObj.value("x", 1.0f);
        scale.y = scaleObj.value("y", 1.0f);
        scale.z = scaleObj.value("z", 1.0f);
        gameObject->SetScale(scale);
    }

    gameObject->LoadProperties(jsonObject);

    if (jsonObject.contains("components") && jsonObject["components"].is_array())
    {
        const auto& components = jsonObject["components"];
        for (const auto& compJson : components)
        {
            const auto type      = compJson.value("type", "");
            auto*      component = ComponentFactory::GetInstance().CreateComponent(type);

            if (component != nullptr)
            {
                component->LoadProperties(compJson);
                gameObject->AddComponent(component);
            }
        }
    }

    if (jsonObject.contains("children") && jsonObject["children"].is_array())
    {
        const auto& children = jsonObject["children"];
        for (const auto& child : children)
        {
            LoadObject(child, gameObject);
        }
    }

    gameObject->Init();
}

} // namespace engine