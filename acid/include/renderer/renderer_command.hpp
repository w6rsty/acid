#pragma once

#include "renderer_api.hpp"

#include "glm/glm.hpp"

namespace acid
{

class RendererCommand
{
public:
    static void Init()
    {
        rendererAPI_->Init();
    }

    static void Shutdown()
    {
        rendererAPI_->Shutdown();
    }

    static void SetClearColor(const glm::vec4& color)
    {
        rendererAPI_->SetClearColor(color);
    }

    static void Clear()
    {
        rendererAPI_->Clear();
    }

    static void DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        rendererAPI_->DrawIndexed(vertexArray);
    }
private:
    static RendererAPI* rendererAPI_;
};

} // namespace acid