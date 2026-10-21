#pragma once

#include "engine_includes.h"

class JumpPlatform : public engine::GameObject, public engine::IContactListener
{
    GAMEOBJECT(JumpPlatform);

  public:
    void Init() override;
    void OnContact(engine::CollisionObject* obj, const glm::vec3& pos,
                   const glm::vec3& norm) override;
};