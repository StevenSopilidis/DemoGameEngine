#pragma once

namespace engine
{
class Application
{
  public:
    virtual bool Init() = 0;
    // @param deltaTime: in seconds
    virtual void Update(float deltaTime) = 0;
    virtual void Destroy()               = 0;

    void               SetNeedsToClose(bool value) noexcept;
    [[nodiscard]] bool NeedsToClose() const noexcept;

  private:
    bool needs_to_close_{false};
};
} // namespace engine