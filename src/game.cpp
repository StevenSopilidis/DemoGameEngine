#include "game.h"

#include "GLFW/glfw3.h"
#include "collider.h"
#include "light_component.h"
#include "physics_component.h"
#include "player.h"
#include "rigid_body.h"
#include "test_object.h"

#include <iostream>

void Game::RegisterTypes() { Player::Register(); }

bool Game::Init()
{
    scene_ = engine::Scene::Load("scenes/scene.sc");
    engine::Engine::GetInstance().SetCurrentScene(scene_);
    return true;
}

void Game::Update(float deltaTime)
{
    auto& inputManager = engine::Engine::GetInstance().GetInputManager();

    scene_->Update(deltaTime);
}

void Game::Destroy() {}