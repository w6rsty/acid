#include "renderer/graphic_context.hpp"

#ifdef ACID_PLATFORM_WINDOWS

#include "core/assert.hpp"
#include "window/window.hpp"

#include "GLFW/glfw3.h"

namespace acid
{
    
Ref<Window> Window::Create(const WindowCreateInfo& info)
{
    return CreateRef<Window>(info);
}

Window::Window(const WindowCreateInfo& info)
{
    config_.Title = info.Title;
    config_.Width = info.Width;
    config_.Height = info.Height;
    config_.VSync = info.VSync;

    int GLFWInitStatus = glfwInit();
    AC_ASSERT_MSG(GLFWInitStatus, "Faild to Initialize GLFW");

    GLFWwindow* window = glfwCreateWindow(config_.Width, config_.Height, config_.Title.c_str(), nullptr, nullptr);
    AC_ASSERT_MSG(window, "Failed to create GLFW window");
    window_ = window;

    context_ = GraphicContext::Create(window);
    context_->Init();

}

Window::~Window()
{
    glfwDestroyWindow(static_cast<GLFWwindow*>(window_));
    glfwTerminate();
}

void Window::OnUpdate()
{
    glfwPollEvents();
    context_->Swap();
}

void Window::SetSync(bool enable)
{
    config_.VSync = enable;
    glfwSwapInterval(enable ? 1 : 0);
}

uint32_t Window::GetWidth() const 
{
    return config_.Width;
}

uint32_t Window::GetHeight() const
{
    return config_.Height;
}

void* Window::GetNativeWindow() const
{
    return window_;
}

} // namespace acid

#endif

