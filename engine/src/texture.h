#pragma once

#include "GL/glew.h"

#include <memory>

namespace engine
{
class Texture
{
  public:
    Texture(int width, int height, int num_channels, unsigned char* data);
    ~Texture();

    [[nodiscard]] GLuint Id() const;

    static std::shared_ptr<Texture> Load(const std::string& path);

  private:
    void Init(int width, int height, int num_channels, unsigned char* data);

    GLuint texture_id_{0};
    int    width_{0};
    int    height_{0};
    int    num_channels_{0};
};
} // namespace engine