#include "engine_includes.h"
#include "game.h"

#include <iostream>

int main()
{
    auto& engine = engine::Engine::GetInstance();
    engine.SetApplication(new Game());

    if (engine.Init(1280, 720))
    {
        std::cout << "Engine initialized successfully\n";
        engine.Run();
    }

    engine.Destroy();
}