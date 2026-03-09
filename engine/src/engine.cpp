#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "app.h"

#include <iostream>

void GLFWwindowDeleter::operator()(GLFWwindow* window) const
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
}

namespace engine
{

Engine& Engine::GetInstance()
{
    static Engine instance;
    return instance;
}

void keyCallback(GLFWwindow* window, int key, int, int action, int)
{
    auto& manager = Engine::GetInstance().GetInputManager();
    if (action == GLFW_PRESS)
    {
        manager.SetKeyPressed(static_cast<std::size_t>(key), true);
    }
    else if (action == GLFW_RELEASE)
    {
        manager.SetKeyPressed(static_cast<std::size_t>(key), false);
    }
}

bool Engine::Init(int window_width, int window_height)
{
    if (!application_)
    {
        return false;
    }

#if defined(__linux__)
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

    if (!glfwInit())
    {
        std::cout << "glfwInit() failed\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_.reset(
        glfwCreateWindow(window_width, window_height, "FirstAssignmentWindow", nullptr, nullptr));

    if (!window_)
    {
        std::cout << "glfwCreateWindow() failed\n";
        return false;
    }

    glfwSetKeyCallback(window_.get(), keyCallback);

    glfwMakeContextCurrent(window_.get());

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewInit() failed\n";
        glfwTerminate();
        return false;
    }

    return application_->Init();
}

void Engine::Run()
{
    if (!application_)
    {
        return;
    }

    last_time_point_ = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window_.get()) && !application_->NeedsToClose())
    {
        glfwPollEvents();

        auto now         = std::chrono::high_resolution_clock::now();
        auto deltaTime   = std::chrono::duration<float>(now - last_time_point_).count();
        last_time_point_ = now;
        application_->Update(deltaTime);

        glfwSwapBuffers(window_.get());
    }
}

void Engine::Destroy()
{
    if (application_)
    {
        application_->Destroy();
        application_.reset(nullptr);
        glfwTerminate();
    }
}

void Engine::SetApplication(Application* app) noexcept { application_.reset(app); }

Application* Engine::GetApplication() const noexcept { return application_.get(); }

InputManager& Engine::GetInputManager() { return input_manager_; }

GraphicsApi& Engine::GetGraphicsApi() { return graphics_api_; };

} // namespace engine