#include "engine/engine.hpp"

#include "geometry/geo.hpp"
#include "renderer/camera/scene_camera.hpp"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

namespace acid
{

Engine::Engine()
{
    Init();
} 

Engine::~Engine()
{
    running_ = false;
}

void Engine::Init()
{
    window_ = Window::Create({});

    vertexArray_ = VertexArray::Create();

    auto info = geo::Box::UnitData();
    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(info.Vertices.data(), info.VertexSize);
    vertexBuffer->SetLayout({
        { VertexDataType::Float3, "a_Position" }
    });
    vertexArray_->SetVertexBuffer(vertexBuffer);
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(info.Indices.data(), info.Indices.size());
    vertexArray_->SetIndexBuffer(indexBuffer);

    shader_ = Shader::Create("assets/shaders/plain.shader");
    shader_->Bind();
    shader_->SetUniformFloat4("u_Color", { 0.2f, 0.3f, 0.8f, 1.0f });

    camera_ = CreateRef<SceneCamera>();
    camera_->SetPerspective(1280.0f / 720.0f, 45.0f, 0.01f, 1000.0f);
    // camera_->SetOrthographic(1280.0f / 720.0f, 2.0f, -2.0f, 2.0f);
    camera_->SetPosition({0.0f, 0.0f, 2.0f});
    camera_->SetRotation({0.0f, 45.0f, 0.0f});
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_->Bind();
        shader_->SetUniformMat4("u_Projection", camera_->GetProjectionMatrix());
        shader_->SetUniformMat4("u_View", camera_->GetViewMatrix());
        vertexArray_->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray_->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

        window_->OnUpdate(); 
    }
}

} // namespace acid