#include "engine/application.hpp"

#include "core/log.hpp"
#include "ImGui/imgui_utils.hpp"

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
    engine_ = CreateRef<Engine>();
}

void Application::Run()
{
    while (engine_->running_)
    {
        ImGuiLayer::Begin();
        engine_->Run();
        ImGuiLayer::End();
        window_->OnUpdate();
    }
}

Application* Application::sInstance_ = nullptr;

} // namespace