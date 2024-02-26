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

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};

} // namespace acid