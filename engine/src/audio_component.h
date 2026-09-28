#pragma once

#include "audio.h"
#include "component.h"

namespace engine
{

class AudioComponent : public Component
{
    COMPONENT(AudioComponent)
  public:
    void LoadProperties(const nlohmann::json& json) override;
    void Update(float deltaTime) override;

    void RegisterAudio(const std::string& name, std::shared_ptr<Audio>& clip);
    void Play(const std::string& name, bool loop = false);
    void Stop(const std::string& name);
    bool IsPlaying(const std::string& name);

  private:
    std::unordered_map<std::string, std::shared_ptr<Audio>> clips_;
};

} // namespace engine