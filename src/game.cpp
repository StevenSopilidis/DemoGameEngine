#include "game.h"

#include "GLFW/glfw3.h"
#include "collider.h"
#include "light_component.h"
#include "physics_component.h"
#include "rigid_body.h"
#include "test_object.h"

#include <iostream>

bool Game::Init()
{
    auto& fs      = engine::Engine::GetInstance().GetFs();
    auto  texture = engine::Texture::Load("textures/brick.png");
    if (!texture)
    {
        std::cout << "Failed to load brick.png\n";
        return false;
    }

    auto material = engine::Material::Load("materials/brick.mat.json");
    if (material == nullptr)
    {
        std::cerr << "material/brick.mat does not exist";
        return false;
    }

    auto mesh = engine::Mesh::CreateBox();

    scene_ = new engine::Scene();
    engine::Engine::GetInstance().SetCurrentScene(scene_);

    auto* camera = scene_->CreateObject("Camera");
    camera->AddComponent(new engine::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new engine::PlayerControllerComponent());

    scene_->SetMainCamera(camera);

    scene_->CreateObject<TestObject>("TestObject");

    auto* suzanneObject = engine::GameObject::LoadGLTF("models/suzanne/Suzanne.gltf");
    suzanneObject->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

    auto* gunObject = engine::GameObject::LoadGLTF("models/sten_gunmachine_carbine/scene.gltf");
    gunObject->SetParent(camera);
    gunObject->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gunObject->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    if (auto anim = gunObject->GetComponent<engine::AnimationComponent>())
    {
        if (auto bullet = gunObject->FindChildByName("bullet_33"))
        {
            bullet->SetActive(true);
        }
        if (auto file = gunObject->FindChildByName("BOOM_35"))
        {
            file->SetActive(true);
        }

        anim->Play("shoot");
    }
    else
    {
        std::cout << "Could not get animation\n";
    }

    auto* light     = scene_->CreateObject("Light");
    auto* lightComp = new engine::LightComponent();
    lightComp->SetColor(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    auto* ground = scene_->CreateObject("Ground");
    ground->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

    glm::vec3 groundExtents(20.0f, 2.0f, 20.0f);
    auto      groundMesh = engine::Mesh::CreateBox(groundExtents);
    ground->AddComponent(new engine::MeshComponent(material, groundMesh));

    auto groundCollider = std::make_shared<engine::BoxCollider>(groundExtents);
    auto groundBody =
        std::make_shared<engine::RigidBody>(engine::BodyType::Static, groundCollider, 0.0f, 0.5f);
    ground->AddComponent(new engine::PhysicsComponent(groundBody));

    auto* boxObj = scene_->CreateObject("FallingBox");
    boxObj->AddComponent(new engine::MeshComponent(material, mesh));
    boxObj->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    boxObj->SetRotation(glm::quat(glm::vec3(1.0f, 2.0f, 0.0f)));
    auto boxCollider = std::make_shared<engine::BoxCollider>(glm::vec3(1.0f));
    auto boxBody =
        std::make_shared<engine::RigidBody>(engine::BodyType::Dynamic, boxCollider, 5.0f, 0.5f);
    boxObj->AddComponent(new engine::PhysicsComponent(boxBody));

    camera->SetPosition(glm::vec3(0.0f, 1.0f, 7.0f));

    return true;
}

void Game::Update(float deltaTime)
{
    auto& inputManager = engine::Engine::GetInstance().GetInputManager();

    scene_->Update(deltaTime);
}

void Game::Destroy() {}