#include "engine/engine.hpp"

#include "renderer/renderer.hpp"
#include "renderer/renderer_command.hpp"

namespace acid
{

Engine::Engine()
{
    Init(); // Init Engine self
    Renderer::Init(); // Init Renderer
} 

Engine::~Engine()
{
    running_ = false;
}

void Engine::Init()
{
    window_ = Window::Create({}); /// TODO: move this to application

    camera_ = CreateRef<SceneCamera>();
    camera_->SetPerspective(1280.0f / 720.0f, 30.0f, 0.01f, 1000.0f);
    camera_->SetPosition({0.0f, 0.0f, 5.0f});

    // RendererCommand::DrawWireFrame(true);
}

void Engine::OnAttach()
{

}

void Engine::OnDetach()
{

}


void Engine::Run()
{
    while (running_)
    {
        RendererCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RendererCommand::Clear();

        Renderer3D::BeginScene(camera_);

        Renderer3D::DrawCuboid();

        Renderer3D::EndScene();

        window_->OnUpdate(); 
    }
}

} // namespace acid