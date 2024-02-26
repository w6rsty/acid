#include "engine/engine.hpp"

#include "geometry/geo.hpp"
#include "renderer/camera/scene_camera.hpp"

#include "renderer/renderer.hpp"
#include "renderer/renderer_command.hpp"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer/renderer_enum.hpp"

namespace acid
{

Engine::Engine()
{
    Init();
    Renderer::Init();
} 

Engine::~Engine()
{
    running_ = false;
}

void Engine::Init()
{
    window_ = Window::Create({});

    vertexArray_ = VertexArray::Create();

    auto drawInfo = geo::Capsule::UnitData();
    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(drawInfo.Vertices.data(), drawInfo.VertexSize);
    vertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" },
        { VertexDataType::Float3, "a_Normal" },
        { VertexDataType::Float2, "a_TexCoord" }
    });
    vertexArray_->SetVertexBuffer(vertexBuffer);
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(drawInfo.Indices.data(), drawInfo.Indices.size());
    vertexArray_->SetIndexBuffer(indexBuffer);

    shader_ = Shader::Create("assets/shaders/plain.shader");
    shader_->Bind();
    shader_->SetUniformFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 1.0f });

    camera_ = CreateRef<SceneCamera>();
    camera_->SetPerspective(1280.0f / 720.0f, 30.0f, 0.01f, 1000.0f);
    // camera_->SetOrthographic(1280.0f / 720.0f, 2.0f, -2.0f, 2.0f);
    camera_->SetPosition({0.0f, 0.0f, 3.0f});
    camera_->SetRotation({0.0f, 0.0f, 0.0f});
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

        shader_->Bind();
        shader_->SetUniformMat4("u_Projection", camera_->GetProjectionMatrix());
        shader_->SetUniformMat4("u_View", camera_->GetViewMatrix());
        Renderer::Submit(shader_, vertexArray_);

        window_->OnUpdate(); 
    }
}

} // namespace acid