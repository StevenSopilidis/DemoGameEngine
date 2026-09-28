#include "audio_component.h"

#include "game_object.h"

namespace engine
{

void AudioComponent::LoadProperties(const nlohmann::json& json)
{
    if (json.contains("audio"))
    {
        const auto& clipsObject = json["audio"];
        for (const auto& clip : clipsObject)
        {
            auto name  = clip.value("name", "noname");
            auto path  = clip.value("path", "");
            auto audio = Audio::Load(path);
            if (audio)
            {
                auto volume = clip.value("volume", 1.0f);
                audio->SetVolume(volume);
                RegisterAudio(name, audio);
            }
        }
    }
}

void AudioComponent::Update(float deltaTime)
{
    auto pos = owner_->GetWorldPosition();
    for (auto& clip : clips_)
    {
        if (clip.second->IsPlaying())
        {
            clip.second->SetPosition(pos);
        }
    }
}

void AudioComponent::RegisterAudio(const std::string& name, std::shared_ptr<Audio>& clip)
{
    clips_[name] = clip;
}

void AudioComponent::Play(const std::string& name, bool loop)
{
    auto it = clips_.find(name);
    if (it != clips_.end())
    {
        if (it->second)
        {
            it->second->Play(loop);
        }
    }
}

void AudioComponent::Stop(const std::string& name)
{
    auto it = clips_.find(name);
    if (it != clips_.end())
    {
        if (it->second)
        {
            it->second->Stop();
        }
    }
}

bool AudioComponent::IsPlaying(const std::string& name)
{
    auto it = clips_.find(name);
    if (it != clips_.end())
    {
        if (it->second)
        {
            return it->second->IsPlaying();
        }
    }

    return false;
}

} // namespace engine