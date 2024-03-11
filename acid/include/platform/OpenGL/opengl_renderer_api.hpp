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
    virtual void DrawIndexedArray(const Ref<VertexArray>& vertedxArray, uint32_t indexCount, uint32_t count) override;
    virtual void DrawArray(const Ref<VertexArray>& vertexArray, uint32_t count) override;
};

} // namespace acid