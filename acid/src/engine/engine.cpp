#include "engine/engine.hpp"

#include "renderer/renderer.hpp"
#include "renderer/renderer_command.hpp"

#include "glm/ext/matrix_transform.hpp"

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

        // create a x-z terrain
        for (int x = -10; x < 10; x++)
        {
            for (int z = -10; z < 0; z++)
            {
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), {x * 2.0f, 0.0f, z * 2.0f});
                Renderer3D::DrawQuad(transform);
            }
        }


        Renderer3D::EndScene();

        window_->OnUpdate(); 
    }
}

} // namespace acid