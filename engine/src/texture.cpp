#include "texture.h"

#include "engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace engine
{
Texture::Texture(int width, int height, int num_channels, unsigned char* data)
    : width_{width}, height_{height}, num_channels_{num_channels}
{
    Init(width, height, num_channels, data);
}

Texture::~Texture()
{
    if (texture_id_ > 0)
    {
        glDeleteTextures(1, &texture_id_);
    }
}

GLuint Texture::Id() const { return texture_id_; }

std::shared_ptr<Texture> Texture::Load(const std::string& path)
{
    int width, height, num_channels;

    auto& fs        = Engine::GetInstance().GetFs();
    auto  full_path = fs.GetAssetsFolder() / path;

    if (!std::filesystem::exists(full_path))
    {
        return nullptr;
    }

    std::shared_ptr<Texture> result;

    unsigned char* data = stbi_load(full_path.string().c_str(), &width, &height, &num_channels, 0);

    if (data)
    {
        result = std::make_shared<Texture>(width, height, num_channels, data);
        stbi_image_free(data);
    }

    return result;
}

void Texture::Init(int width, int height, int num_channels, unsigned char* data)
{
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

} // namespace engine