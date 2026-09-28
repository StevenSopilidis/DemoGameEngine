#include "audio_manager.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace engine
{
AudioManager::AudioManager() { engine_ = std::make_unique<ma_engine>(); }

AudioManager::~AudioManager()
{
    if (engine_)
    {
        ma_engine_uninit(engine_.get());
    }
}

bool AudioManager::Init()
{
    auto result = ma_engine_init(nullptr, engine_.get());
    return result == MA_SUCCESS;
}

ma_engine* AudioManager::GetEngine() { return engine_.get(); }

void AudioManager::SetListenerPosition(const glm::vec3& pos)
{
    if (engine_)
    {
        ma_engine_listener_set_position(engine_.get(), 0, pos.x, pos.y, pos.z);
    }
}

} // namespace engine