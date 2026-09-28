#include "audio_listener_component.h"

#include "engine.h"
#include "game_object.h"

namespace engine
{

void AudioListenerComponent::Update(float deltaTime)
{
    auto pos = owner_->GetWorldPosition();
    Engine::GetInstance().GetAudioManager().SetListenerPosition(pos);
}

} // namespace engine
