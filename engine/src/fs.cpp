#include "fs.h"

#include "config.h"

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
std::filesystem::path Fs::GetExecutableFolder() const
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

std::filesystem::path Fs::GetAssetsFolder() const
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

} // namespace engine