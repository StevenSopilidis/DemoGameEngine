#include "engine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "app.h"
#include "camera_component.h"
#include "component.h"

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

RenderQueue& Engine::GetRenderQueue() noexcept { return render_queue_; }

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

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto& manager = Engine::GetInstance().GetInputManager();
    if (action == GLFW_PRESS)
    {
        manager.SetMouseButtonPressed(button, true);
    }
    else if (action == GLFW_RELEASE)
    {
        manager.SetMouseButtonPressed(button, false);
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto& manager = Engine::GetInstance().GetInputManager();
    manager.SetMousePositionOld(manager.MousePositionCurr());

    glm::vec2 currentPos(static_cast<float>(xpos), static_cast<float>(ypos));
    manager.SetMousePositionCurr(currentPos);
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
    glfwSetMouseButtonCallback(window_.get(), mouseButtonCallback);
    glfwSetCursorPosCallback(window_.get(), cursorPositionCallback);

    glfwMakeContextCurrent(window_.get());

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewInit() failed\n";
        glfwTerminate();
        return false;
    }

    graphics_api_.Init();
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

        graphics_api_.SetClearColor(1.f, 1.f, 1.f, 1.f);
        graphics_api_.ClearBuffers();

        CameraData cameraData;

        int width;
        int height;
        glfwGetWindowSize(window_.get(), &width, &height);
        auto aspect = static_cast<float>(width) / static_cast<float>(height);

        if (current_scene_)
        {
            if (auto* cameraObject = current_scene_->MainCamera())
            {
                auto* cameraComponent = cameraObject->GetComponent<CameraComponent>();
                if (cameraComponent != nullptr)
                {
                    cameraData.viewMatrix       = cameraComponent->GetViewMatrix();
                    cameraData.projectionMatrix = cameraComponent->GetProjectionMatrix(aspect);
                }
            }
        }

        render_queue_.Draw(graphics_api_, cameraData);

        glfwSwapBuffers(window_.get());

        input_manager_.SetMousePositionOld(input_manager_.MousePositionCurr());
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

Scene* Engine::CurrentScene() { return current_scene_.get(); }

void Engine::SetCurrentScene(Scene* scene) { current_scene_.reset(scene); }

} // namespace engine