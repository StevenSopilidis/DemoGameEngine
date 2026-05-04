#include "game.h"

#include "test_object.h"

bool Game::Init()
{
    scene_.CreateObject<TestObject>("TestObject");
    return true;
}

void Game::Update(float deltaTime) { scene_.Update(deltaTime); }

void Game::Destroy() {}