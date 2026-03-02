#include "engine_includes.h"
#include "game.h"

int main()
{
    auto& engine = engine::Engine::GetInstance();
    engine.SetApplication(new Game());

    if (engine.Init(1280, 720))
    {
        engine.Run();
    }

    engine.Destroy();
}