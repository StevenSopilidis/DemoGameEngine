#include "app.h"

namespace engine
{
void Application::SetNeedsToClose(bool value) noexcept { needs_to_close_ = value; }

bool Application::NeedsToClose() const noexcept { return needs_to_close_; }
} // namespace engine