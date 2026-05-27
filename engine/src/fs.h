#pragma once

#include <filesystem>
namespace engine
{
class Fs
{
  public:
    [[nodiscard]] std::filesystem::path GetExecutableFolder() const; 
    [[nodiscard]] std::filesystem::path GetAssetsFolder() const;
};
} // namespace engine