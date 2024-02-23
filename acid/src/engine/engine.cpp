#include "engine/engine.hpp"

#include "glad/glad.h"

namespace acid
{

Engine::Engine()
{
    window_ = Window::Create({});

    vertexArray_ = VertexArray::Create();
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };
    Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
    vertexBuffer->SetLayout({
        { VertexDataType::Float2, "a_Position"}
    });
    vertexArray_->SetVertexBuffer(vertexBuffer);
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    vertexArray_->SetIndexBuffer(indexBuffer);
}

Engine::~Engine()
{

}

void Engine::Init()
{

}

void Engine::OnAttach()
{

}

void Engine::OnDetach()
{

}


void Engine::Run()
{
    while (1)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        vertexArray_->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray_->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

        window_->OnUpdate();
    }
}

} // namespace acid