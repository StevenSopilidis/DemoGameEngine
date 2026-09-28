#include "common.h"

#include <filesystem>
#include <memory>

struct ma_sound;
struct ma_decoder;

namespace engine
{
class Audio
{
  public:
    ~Audio();
    void                SetPosition(const glm::vec3& position);
    void                Play(bool loop = false);
    void                Stop();
    [[nodiscard]] bool  IsPlaying() const;
    void                SetVolume(float volume);
    [[nodiscard]] float GetVolume() const;

    static std::shared_ptr<Audio> Load(const std::filesystem::path& path);

  private:
    std::unique_ptr<ma_sound>   sound_;
    std::unique_ptr<ma_decoder> decoder_;
    std::vector<char>           buffer_;
};
} // namespace engine