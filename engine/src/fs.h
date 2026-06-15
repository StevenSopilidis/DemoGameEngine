#pragma once

#include <filesystem>
#include <vector>
namespace engine
{
class Fs
{
  public:
    static std::filesystem::path GetExecutableFolder();
    static std::filesystem::path GetAssetsFolder();

    static std::vector<char> LoadFile(const std::filesystem::path& path);
    static std::vector<char> LoadAssetFile(const std::filesystem::path& relative_path);
    static std::string       LoadAssetFileText(const std::filesystem::path& relative_path);
};
} // namespace engine