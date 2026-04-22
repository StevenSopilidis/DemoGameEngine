#include "scene.h"

#include <algorithm>

namespace engine
{
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
    SetParent(obj, parent);

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

} // namespace engine