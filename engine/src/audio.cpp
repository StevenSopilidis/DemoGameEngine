#include "audio.h"

#include "engine.h"
#include "miniaudio.h"

namespace engine
{
Audio::~Audio()
{
    if (sound_)
    {
        ma_sound_uninit(sound_.get());
    }

    if (decoder_)
    {
        ma_decoder_uninit(decoder_.get());
    }
}

std::shared_ptr<Audio> Audio::Load(const std::filesystem::path& path)
{
    auto  buffer = Engine::GetInstance().GetFs().LoadAssetFile(path);
    auto* engine = Engine::GetInstance().GetAudioManager().GetEngine();

    auto audio      = std::make_shared<Audio>();
    audio->sound_   = std::make_unique<ma_sound>();
    audio->buffer_  = buffer;
    audio->decoder_ = std::make_unique<ma_decoder>();
    auto result     = ma_decoder_init_memory(audio->buffer_.data(), audio->buffer_.size(), nullptr,
                                             audio->decoder_.get());

    if (result != MA_SUCCESS)
    {
        return nullptr;
    }

    result =
        ma_sound_init_from_data_source(engine, audio->decoder_.get(), 0, NULL, audio->sound_.get());

    if (result != MA_SUCCESS)
    {
        return nullptr;
    }

    ma_sound_set_spatialization_enabled(audio->sound_.get(), MA_TRUE);
    return audio;
}

void Audio::SetPosition(const glm::vec3& position)
{
    if (sound_)
    {
        ma_sound_set_position(sound_.get(), position.x, position.y, position.z);
    }
}

void Audio::Play(bool loop)
{
    if (sound_)
    {
        ma_sound_start(sound_.get());
        ma_sound_set_looping(sound_.get(), loop ? MA_TRUE : MA_FALSE);
    }
}

void Audio::Stop()
{
    if (sound_)
    {
        ma_sound_stop(sound_.get());
        ma_sound_seek_to_pcm_frame(sound_.get(), 0);
    }
}

void Audio::SetVolume(float volume)
{
    if (sound_)
    {
        ma_sound_set_volume(sound_.get(), volume);
    }
}

bool Audio::IsPlaying() const
{
    if (sound_)
    {
        return ma_sound_is_playing(sound_.get());
    }
    return false;
}

float Audio::GetVolume() const
{
    if (sound_)
    {
        return ma_sound_get_volume(sound_.get());
    }

    return 0.0f;
}

} // namespace engine