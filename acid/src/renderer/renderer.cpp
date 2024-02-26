#include "renderer/renderer.hpp"

#include "renderer/renderer_command.hpp"

namespace acid
{

void Renderer::Init()
{
    RendererCommand::Init();
}

void Renderer::Shutdown()
{
    RendererCommand::Shutdown();
}

void Renderer::BeginScene(const Ref<SceneCamera> &camera)
{

}

void Renderer::EndScene()
{
    
}

void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray)
{
    RendererCommand::DrawIndexed(vertexArray);
}

} // namespace acid