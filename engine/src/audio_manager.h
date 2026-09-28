#pragma once

#include "common.h"

#include <memory>

struct ma_engine;

namespace engine
{
class AudioManager
{
  public:
    AudioManager();
    ~AudioManager();

    bool Init();

    ma_engine* GetEngine();
    void       SetListenerPosition(const glm::vec3& pos);

  private:
    std::unique_ptr<ma_engine> engine_;
};
} // namespace engine