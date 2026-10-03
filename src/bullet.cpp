#include "bullet.h"

void Bullet::Update(float deltaTime)
{
    engine::GameObject::Update(deltaTime);
    life_time_ -= deltaTime;

    if (life_time_ <= 0.0f)
    {
        MarkForDestroy();
    }
}