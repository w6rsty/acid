#pragma once

#include "renderer/renderer_api.hpp"

namespace acid
{

class OpenGLRendererAPI final : public RendererAPI
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;
    virtual void DrawWireFrame(bool enable = true) override;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
};

} // namespace acid