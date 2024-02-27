#include "engine/engine.hpp"

#include "renderer/renderer.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/renderer_command.hpp"

#include "glm/ext/matrix_transform.hpp"

// demostration
#include "GLFW/glfw3.h"

namespace acid
{

Engine::Engine()
{
    Init(); // Init Engine self
    Renderer::Init(); // Init Renderer(static)
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
    camera_->SetPosition({4.0f, 0.0f, 5.0f});
    camera_->SetRotation({0.0f, 45.0f, 0.0f});

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

        Renderer::BeginScene(camera_);

        Renderer::DrawCuboid(glm::mat4(1.0f));
        Renderer::DrawCuboid(glm::translate(glm::mat4(1.0f), {-1.0f,  0.0f, 0.0f}));
        Renderer::DrawCuboid(glm::translate(glm::mat4(1.0f), { 1.0f,  0.0f, 0.0f}));
        Renderer::DrawCuboid(glm::translate(glm::mat4(1.0f), { 0.0f,  1.0f, 0.0f}));
        Renderer::DrawCuboid(glm::translate(glm::mat4(1.0f), { 0.0f, -1.0f, 0.0f}));

        Renderer::EndScene();

        window_->OnUpdate(); 
    }
}

} // namespace acid