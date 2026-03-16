#pragma once

#include "graphics_api.h"
#include "input.h"
#include "render_queue.h"

#include <chrono>
#include <memory>

struct GLFWwindow;

struct GLFWwindowDeleter
{
    void operator()(GLFWwindow* window) const;
};

namespace engine
{
class Application;

class Engine
{
  public:
    Engine(const Engine&)            = delete;
    Engine(Engine&&)                 = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&)      = delete;

    static Engine& GetInstance();

    bool Init(int window_width, int window_height);
    void Run();
    void Destroy();

    void                       SetApplication(Application* app) noexcept;
    [[nodiscard]] Application* GetApplication() const noexcept;
    [[nodiscard]] RenderQueue& GetRenderQueue() noexcept;

    InputManager& GetInputManager();
    GraphicsApi&  GetGraphicsApi();

  private:
    using GLFWwindowPtr = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>;

    Engine() = default;

    std::unique_ptr<Application>                   application_;
    std::chrono::high_resolution_clock::time_point last_time_point_;
    GLFWwindowPtr                                  window_;
    InputManager                                   input_manager_;
    GraphicsApi                                    graphics_api_;
    RenderQueue                                    render_queue_;
};
} // namespace engine