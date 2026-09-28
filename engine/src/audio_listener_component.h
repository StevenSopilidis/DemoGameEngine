#pragma once

#include "component.h"

namespace engine
{

class AudioListenerComponent : public Component
{
    COMPONENT(AudioListenerComponent)

  public:
    void Update(float deltaTime) override;
};

} // namespace engine