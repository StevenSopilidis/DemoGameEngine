#include "physics_component.h"

#include "engine.h"
#include "game_object.h"

namespace engine
{

PhysicsComponent::PhysicsComponent(const std::shared_ptr<RigidBody>& body) : rigidBody_(body) {}

void PhysicsComponent::Init()
{
    if (!rigidBody_)
    {
        return;
    }

    const auto pos = owner_->GetWorldPosition();
    const auto rot = owner_->GetWorldRotation();

    rigidBody_->SetPosition(pos);
    rigidBody_->SetRotation(rot);

    Engine::GetInstance().GetPhysicsManager().AddRigidBody(rigidBody_.get());
}

void PhysicsComponent::Update(float deltaTime)
{
    if (!rigidBody_)
    {
        return;
    }

    if (rigidBody_->Type() == BodyType::Dynamic)
    {
        owner_->SetWorldPosition(rigidBody_->Position());
        owner_->SetWorldRotation(rigidBody_->Rotation());
    }
}

void PhysicsComponent::SetRigidBody(const std::shared_ptr<RigidBody>& body) { rigidBody_ = body; }

void PhysicsComponent::LoadProperties(const nlohmann::json& json)
{
    std::shared_ptr<Collider> collider{};

    if (json.contains("collider"))
    {
        const auto& colliderObj = json["collider"];
        auto        type        = colliderObj.value("type", "");

        if (type == "box")
        {
            glm::vec3 extents(colliderObj.value("x", 1.0f), colliderObj.value("y", 1.0f),
                              colliderObj.value("z", 1.0f));
            collider = std::make_shared<BoxCollider>(extents);
        }
        else if (type == "sphere")
        {
            float radius = colliderObj.value("r", 1.0f);
            collider     = std::make_shared<SphereCollider>(radius);
        }
        else if (type == "capsule")
        {
            float radius = colliderObj.value("r", 1.0f);
            float height = colliderObj.value("r", 1.0f);
            collider     = std::make_shared<CapsuleCollider>(radius, height);
        }

        if (collider == nullptr)
        {
            return;
        }

        std::shared_ptr<RigidBody> rigidBody;
        if (json.contains("body"))
        {
            const auto& bodyObj = json["body"];

            float mass     = bodyObj.value("mass", 0.0f);
            float friction = bodyObj.value("friction", 0.5f);
            auto  typeStr  = bodyObj.value("type", "static");

            auto type = BodyType::Static;

            if (typeStr == "dynamic")
            {
                type = BodyType::Dynamic;
            }
            else if (typeStr == "kinematic")
            {
                type = BodyType::Kinematic;
            }

            rigidBody = std::make_shared<RigidBody>(type, collider, mass, friction);
        }

        if (rigidBody)
        {
            SetRigidBody(rigidBody);
        }
    }
}

} // namespace engine