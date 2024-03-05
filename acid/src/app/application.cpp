#include "app/application.hpp"

#include "core/log.hpp"
#include "ImGui/imgui_utils.hpp"
#include "window/window.hpp"

namespace acid
{

Application::Application()
{
    sInstance_ = this;
    Init();
    ImGuiLayer::Init();
}

Application::~Application()
{

}

void Application::Init()
{
    window_ = Window::Create({});
    engine_ = CreateRef<RenderEngine>();
}

void Application::Run()
{
    while (engine_->running_)
    {
        if (engine_->ready_)
        {
            ImGuiLayer::Begin();
            engine_->Run();
            ImGuiLayer::End();
        }
        window_->OnUpdate();
    }
}

Application* Application::sInstance_ = nullptr;

} // namespace