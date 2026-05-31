#include "fs.h"

#include "config.h"

#include <fstream>
#include <iostream>

#if defined _WIN32
#include "window.h"
#elif defined(__APPLE__)
#include "mach-o/dyld.h"
#elif defined(__linux__)
#include "unistd.h"

#include <climits>
#endif

namespace engine
{
std::filesystem::path Fs::GetExecutableFolder()
{
#if defined _WIN32
    wchar_t buff[MAX_PATH];
    GetModuleFileNameW(NULL, buff, MAX_PATH);
    return std::filesystem::path(buff).remove_filename();
#elif defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string tmp(size, '\0');
    _NSGetExecutablePath(tmp.data(), &size);
    return std::filesystem::weakly_canonical(std::filesystem::path(tmp)).remove_filename();
#elif defined(__linux__)
    return std::filesystem::weakly_canonical(std::filesystem::read_symlink("/proc/self/exe"))
        .remove_filename();
#else
    return std::filesystem::current_path();
#endif
}

std::filesystem::path Fs::GetAssetsFolder()
{
#if defined(ASSETS_ROOT)
    auto path = std::filesystem::path(std::string(ASSETS_ROOT));
    if (std::filesystem::exists(path))
    {
        return path;
    }
#endif

    return std::filesystem::weakly_canonical(GetExecutableFolder() / "assets");
}

std::vector<char> Fs::LoadFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        return {};
    }

    auto size = file.tellg();
    file.seekg(0);

    std::vector<char> buffer(size);

    if (!file.read(buffer.data(), size))
    {
        return {};
    }

    return buffer;
}

std::vector<char> Fs::LoadAssetFile(const std::string& relative_path)
{
    return LoadFile(GetAssetsFolder() / relative_path);
}

std::string Fs::LoadAssetFileText(const std::string& relative_path)
{
    auto buffer = LoadAssetFile(relative_path);

    return {buffer.begin(), buffer.end()};
}

} // namespace engine